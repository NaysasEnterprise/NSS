#include "../server/SQL_header.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

namespace SQL_Database {

    void addUser(sqlite3* db, const std::string& name, const std::string& password) {
        const char* sqlInsert = "INSERT INTO Users (Name, Password) VALUES (?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "User added successfully!" << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    bool loginUser(sqlite3* db, const std::string& name, const std::string& password) {
        const char* sqlSelect = "SELECT Password FROM Users WHERE Name = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (storedPassword && password == storedPassword) {
                std::cout << "Login successful!" << std::endl;
                sqlite3_finalize(stmt);
                return true;
            }
            else {
                std::cerr << "Invalid password!" << std::endl;
            }
        }
        else {
            std::cerr << "User not found!" << std::endl;
        }

        sqlite3_finalize(stmt);
        return false;
    }
    void deleteUser(sqlite3* db, const std::string& name) {
        const char* sqlDelete = "DELETE FROM Users WHERE Name = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "User deleted successfully!" << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    void displayUsers(sqlite3* db) {
        const char* sqlSelect = "SELECT * FROM Users;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        std::cout << "Users in the database:\n";
        std::cout << "ID\tName\tPassword\n";
        std::cout << "----------------------------------\n";

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            std::cout << id << "\t" << (name ? name : "NULL") << "\t" << (password ? password : "NULL") << "\n";
        }

        sqlite3_finalize(stmt);
    }

    void handleGetUserOrders(sqlite3* db, int clientSocket, const std::string& body) {
        // Извлекаем ID пользователя из тела запроса
        std::string userIdStr = extractData(body, "userId");

        if (userIdStr.empty()) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing userId parameter");
            return;
        }

        try {
            int userId = std::stoi(userIdStr);

            // Создаем SQL-запрос для получения заказов пользователя
            std::string sql = "SELECT OrderID, ProductName, OrderDate, DeliveryDate, Status, PickupPointID "
                "FROM Orders WHERE UserID = ?";
            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to prepare SQL statement");
                return;
            }

            // Привязываем значение userId к запросу
            sqlite3_bind_int(stmt, 1, userId);

            // Выполняем запрос и формируем JSON-ответ
            std::string response = "[";
            bool first = true;

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                if (!first) response += ",";
                response += "{";
                response += "\"OrderID\": " + std::to_string(sqlite3_column_int(stmt, 0)) + ",";
                response += "\"ProductName\": \"" + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) + "\",";
                response += "\"OrderDate\": \"" + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) + "\",";
                response += "\"DeliveryDate\": \"" + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))) + "\",";
                response += "\"Status\": \"" + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))) + "\",";
                response += "\"PickupPointID\": " + std::to_string(sqlite3_column_int(stmt, 5));
                response += "}";
                first = false;
            }
            response += "]";

            sqlite3_finalize(stmt);

            sendHttpResponse(clientSocket, "200 OK", response);
        }
        catch (const std::exception& e) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Invalid userId format");
        }
    }


    void handleHttpRequest(sqlite3* db, int clientSocket, const std::string& httpRequest) {
        std::string method, uri, body;
        size_t methodEnd = httpRequest.find(' ');
        size_t uriEnd = httpRequest.find(' ', methodEnd + 1);
        size_t headerEnd = httpRequest.find("\r\n\r\n");

        method = httpRequest.substr(0, methodEnd);
        uri = httpRequest.substr(methodEnd + 1, uriEnd - methodEnd - 1);
        if (headerEnd != std::string::npos) {
            body = httpRequest.substr(headerEnd + 4); // Тело запроса после заголовков
        }
        //для пользователей
        if (method == "POST" && uri == "/addUser") {
            handleAddUser(db, clientSocket, body);
        }
        else if (method == "POST" && uri == "/loginUser") {
            handleLoginUser(db, clientSocket, body);
        }
        else if (method == "POST" && uri == "/deleteUser") {
            handleDeleteUser(db, clientSocket, body);
        }
        else if (method == "GET" && uri == "/displayUsers") {
            handleDisplayUsers(db, clientSocket);
        }
        else if (method == "GET" && uri == "/getUserOrders") {
            handleGetUserOrders(db, clientSocket, body);
        }
        //для пунктов выдачи
        else if (method == "POST" && uri == "/addPickupPoint") {
            handleAddPickupPoint(db, clientSocket, body);
        }
        else if (method == "POST" && uri == "/editPickupPoint") {
            handleEditPickupPoint(db, clientSocket, body);
        }
        else if (method == "POST" && uri == "/deletePickupPoint") {
            handleDeletePickupPoint(db, clientSocket, body);
        }
        else if (method == "GET" && uri == "/displayPickupPoints") {
            handleDisplayPickupPoints(db, clientSocket);
        }
        else {
            sendHttpResponse(clientSocket, "404 Not Found", "Endpoint not found");
        }
    }
    void handleAddUser(sqlite3* db, int clientSocket, const std::string& body) {
        std::string name = extractData(body, "name");
        std::string password = extractData(body, "password");

        if (name.empty() || password.empty()) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing name or password");
            return;
        }

        addUser(db, name, password);
        sendHttpResponse(clientSocket, "200 OK", "User added successfully");
    }
    void handleLoginUser(sqlite3* db, int clientSocket, const std::string& body) {
        std::string name = extractData(body, "name");
        std::string password = extractData(body, "password");

        if (name.empty() || password.empty()) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing name or password");
            return;
        }

        if (loginUser(db, name, password)) {
            sendHttpResponse(clientSocket, "200 OK", "Login successful");
        }
        else {
            sendHttpResponse(clientSocket, "401 Unauthorized", "Invalid credentials");
        }
    }
    void handleDeleteUser(sqlite3* db, int clientSocket, const std::string& body) {
        std::string name = extractData(body, "name");

        if (name.empty()) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing name");
            return;
        }

        deleteUser(db, name);
        sendHttpResponse(clientSocket, "200 OK", "User deleted successfully");
    }
    void handleDisplayUsers(sqlite3* db, int clientSocket) {
        std::stringstream response;
        response << "ID\tName\tPassword\n";
        response << "----------------------------------\n";

        const char* sqlSelect = "SELECT * FROM Users;";
        sqlite3_stmt* stmt;
        if (sqlite3_open("sql_db.db", &db) != SQLITE_OK) {
            std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        }
        if (sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            sendHttpResponse(clientSocket, "500 Internal Server Error", "Database error");
            return;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            response << id << "\t" << (name ? name : "NULL") << "\t" << (password ? password : "NULL") << "\n";
        }

        sendHttpResponse(clientSocket, "200 OK", response.str());
        sqlite3_finalize(stmt);
    }

    // Функция для разбиения SQL-скрипта на отдельные команды
    std::vector<std::string> splitSQLCommands(const std::string& script) {
        std::vector<std::string> commands;
        std::string command;
        std::istringstream stream(script);

        while (std::getline(stream, command, ';')) {
            command = command + ";"; // Восстанавливаем разделитель
            if (!command.empty() && command != ";") {
                commands.push_back(command);
            }
        }
        return commands;
    }
    // Исполнение SQL-скрипта
    void executeSQLScript(sqlite3* db, const std::string& scriptPath) {
        std::ifstream scriptFile(scriptPath);
        if (!scriptFile.is_open()) {
            std::cerr << "Cannot open SQL script file: " << scriptPath << std::endl;
            return;
        }

        std::stringstream buffer;
        buffer << scriptFile.rdbuf();
        std::string sqlScript = buffer.str();

        // Разделение SQL-команд
        auto commands = splitSQLCommands(sqlScript);

        char* errorMessage = nullptr;
        for (const auto& command : commands) {
            if (sqlite3_exec(db, command.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
                std::cerr << "SQL error: " << errorMessage << " for command: " << command << std::endl;
                sqlite3_free(errorMessage);
            }
            else {
                std::cout << "Executed: " << command << std::endl;
            }
        }
    }

    void sendHttpResponse(int clientSocket, const std::string& status, const std::string& body) {
        std::string response = "HTTP/1.1 " + status + "\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
        response += "\r\n";
        response += body;

        send(clientSocket, response.c_str(), response.size(), 0);
    }
    std::string extractData(const std::string& body, const std::string& key) {
        size_t pos = body.find(key + "=");
        if (pos == std::string::npos) return "";
        pos += key.size() + 1;
        size_t end = body.find("&", pos);
        return body.substr(pos, end - pos);
    }
    bool isNumeric(const std::string& str) {
        try {
            std::stod(str);
            return true;
        }
        catch (...) {
            return false;
        }
    }

    //для пунктов выдачи
    void addPickupPoint(sqlite3* db, const std::string& name, const std::string& address, double coordX, double coordY) {
        const char* sqlInsert = "INSERT INTO PickupPoints (Name, Address, CoordX, CoordY) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, address.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, coordX);
        sqlite3_bind_double(stmt, 4, coordY);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "Pickup Point added successfully!" << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    void editPickupPoint(sqlite3* db, int pickupPointID, const std::string& name, const std::string& address, double coordX, double coordY) {
        const char* sqlUpdate = "UPDATE PickupPoints SET Name = ?, Address = ?, CoordX = ?, CoordY = ? WHERE PickupPointID = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlUpdate, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, address.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, coordX);
        sqlite3_bind_double(stmt, 4, coordY);
        sqlite3_bind_int(stmt, 5, pickupPointID);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "Pickup Point updated successfully!" << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    void deletePickupPoint(sqlite3* db, int pickupPointID) {
        const char* sqlDelete = "DELETE FROM PickupPoints WHERE PickupPointID = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        sqlite3_bind_int(stmt, 1, pickupPointID);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "Pickup Point deleted successfully!" << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    void displayPickupPoints(sqlite3* db) {
        const char* sqlSelect = "SELECT * FROM PickupPoints;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        std::cout << "Pickup Points:\n";
        std::cout << "ID\tName\tAddress\tCoordX\tCoordY\n";
        std::cout << "---------------------------------------------------\n";

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            double coordX = sqlite3_column_double(stmt, 3);
            double coordY = sqlite3_column_double(stmt, 4);

            std::cout << id << "\t" << (name ? name : "NULL") << "\t" << (address ? address : "NULL") << "\t"
                << coordX << "\t" << coordY << "\n";
        }

        sqlite3_finalize(stmt);
    }


    //вспомогательные для пунктов выдачи
    void handleAddPickupPoint(sqlite3* db, int clientSocket, const std::string& body) {
        // Извлекаем данные из тела запроса
        std::string name = extractData(body, "name");
        std::string address = extractData(body, "adress");
        std::string coordXStr = extractData(body, "coordX");
        std::string coordYStr = extractData(body, "coordY");

        // Проверяем обязательные поля
        if (name.empty()) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing or invalid 'name'");
            return;
        }
        if (address.empty()) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing or invalid 'adress'");
            return;
        }
        if (coordXStr.empty() || !isNumeric(coordXStr)) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing or invalid 'coordX'");
            return;
        }
        if (coordYStr.empty() || !isNumeric(coordYStr)) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing or invalid 'coordY'");
            return;
        }

        // Преобразуем координаты в числа
        double coordX = std::stod(coordXStr);
        double coordY = std::stod(coordYStr);

        // Выполняем добавление в базу данных
        const char* sqlInsert = "INSERT INTO PickupPoints (Name, Address, CoordX, CoordY) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            sendHttpResponse(clientSocket, "500 Internal Server Error", "Database error");
            return;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, address.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, coordX);
        sqlite3_bind_double(stmt, 4, coordY);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
            sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to add pickup point");
        }
        else {
            sendHttpResponse(clientSocket, "200 OK", "Pickup point added successfully");
        }

        sqlite3_finalize(stmt);
    }
    void handleEditPickupPoint(sqlite3* db, int clientSocket, const std::string& body) {
        std::string idStr = extractData(body, "id");
        std::string name = extractData(body, "name");
        std::string adress = extractData(body, "adress");
        std::string coordXStr = extractData(body, "coordX");
        std::string coordYStr = extractData(body, "coordY");

        if (idStr.empty() || name.empty() || adress.empty() || coordXStr.empty() || coordYStr.empty()) {
            std::cerr << "Missing required parameters. Received:" << std::endl;
            std::cerr << "id: " << idStr << ", name: " << name << ", adress: " << adress
                << ", coordX: " << coordXStr << ", coordY: " << coordYStr << std::endl;
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing required parameters");
            return;
        }

        try {
            int id = std::stoi(idStr);
            double coordX = std::stod(coordXStr);
            double coordY = std::stod(coordYStr);

            // Редактирование пункта ПВЗ
            editPickupPoint(db, id, name, adress, coordX, coordY);
            sendHttpResponse(clientSocket, "200 OK", "Pickup point updated successfully");
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing parameters: " << e.what() << std::endl;
            sendHttpResponse(clientSocket, "400 Bad Request", "Invalid parameter format");
        }
    }
    void handleDeletePickupPoint(sqlite3* db, int clientSocket, const std::string& body) {
        int id = std::stoi(extractData(body, "id"));

        if (id <= 0) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Invalid ID");
            return;
        }

        deletePickupPoint(db, id);
        sendHttpResponse(clientSocket, "200 OK", "Pickup Point deleted successfully");
    }
    void handleDisplayPickupPoints(sqlite3* db, int clientSocket) {
        std::stringstream response;
        response << "Pickup Points:\n";
        response << "ID\tName\tAddress\tCoordX\tCoordY\n";
        response << "----------------------------------\n";

        const char* sqlSelect = "SELECT * FROM PickupPoints;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
            sendHttpResponse(clientSocket, "500 Internal Server Error", "Database error");
            return;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            double coordX = sqlite3_column_double(stmt, 3);
            double coordY = sqlite3_column_double(stmt, 4);

            response << id << "\t" << (name ? name : "NULL") << "\t" << (address ? address : "NULL") << "\t"
                << coordX << "\t" << coordY << "\n";
        }

        sendHttpResponse(clientSocket, "200 OK", response.str());
        sqlite3_finalize(stmt);
    }

}
