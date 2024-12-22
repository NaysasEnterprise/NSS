#include "../server/SQL_header.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>

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

            // Создаем SQL-запрос с JOIN для получения заказов пользователя и имени товара
            std::string sql =
                "SELECT Orders.OrderID, Products.Name AS ProductName, Orders.OrderDate, "
                "Orders.DeliveryDate, Orders.Status, Orders.PickupPointID "
                "FROM Orders "
                "JOIN Products ON Orders.ProductID = Products.ProductID "
                "WHERE Orders.UserID = ?";

            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to prepare SQL statement");
                return;
            }

            // Привязываем значение userId к запросу
            sqlite3_bind_int(stmt, 1, userId);

            // Создаем таблицу в виде строки
            std::string response;
            response += "-------------------------------------------------------------------------------\n";
            response += "| OrderID | ProductName       | OrderDate   | DeliveryDate | Status    | PickupPointID |\n";
            response += "-------------------------------------------------------------------------------\n";

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int orderId = sqlite3_column_int(stmt, 0);

                const unsigned char* productName = sqlite3_column_text(stmt, 1);
                std::string productNameStr = productName ? std::string(reinterpret_cast<const char*>(productName)) : "Unknown";

                const unsigned char* orderDate = sqlite3_column_text(stmt, 2);
                std::string orderDateStr = orderDate ? std::string(reinterpret_cast<const char*>(orderDate)) : "Unknown";

                const unsigned char* deliveryDate = sqlite3_column_text(stmt, 3);
                std::string deliveryDateStr = deliveryDate ? std::string(reinterpret_cast<const char*>(deliveryDate)) : "null";

                const unsigned char* status = sqlite3_column_text(stmt, 4);
                std::string statusStr = status ? std::string(reinterpret_cast<const char*>(status)) : "Unknown";

                int pickupPointId = sqlite3_column_int(stmt, 5);

                // Форматируем строку таблицы
                char row[256];
                snprintf(row, sizeof(row), "| %-7d | %-16s | %-11s | %-12s | %-8s | %-13d |\n",
                    orderId, productNameStr.c_str(), orderDateStr.c_str(), deliveryDateStr.c_str(), statusStr.c_str(), pickupPointId);
                response += row;
            }

            response += "-------------------------------------------------------------------------------\n";

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
        else if (method == "POST" && uri == "/addOrder") {
            handlePlaceOrder(db, clientSocket, body);
        }
        else if (method == "GET" && uri == "/searchOrder") { //для админа скорее всего просто и при большом количестве заказов
            handleSearchOrder(db, clientSocket, body);
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

        else if (method == "GET" && uri == "/getAllOrders") {
            handleGetAllOrders(db, clientSocket);
        }
        else if (method == "GET" && uri == "/getProducts") {
            handleGetProducts(db, clientSocket);
        }
        else if (method == "POST" && uri == "/cancelOrder") {
            handleCancelOrder(db, clientSocket,body);
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

    //заказы
    void handleGetAllOrders(sqlite3* db, int clientSocket) {
        try {
            // SQL-запрос для получения всех записей из таблицы Orders
            std::string sql = R"(
        SELECT OrderID, UserID, ProductID, OrderDate, DeliveryDate, Status, PickupPointID
        FROM Orders;
        )";

            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to prepare SQL statement");
                return;
            }

            // Формируем табличный ответ
            std::string response;
            response += "-------------------------------------------------------------------------------\n";
            response += "| OrderID | UserID | ProductID | OrderDate       | DeliveryDate   | Status    | PickupPointID |\n";
            response += "-------------------------------------------------------------------------------\n";

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Извлекаем данные из строки результата
                int orderId = sqlite3_column_int(stmt, 0);
                int userId = sqlite3_column_int(stmt, 1);
                int productId = sqlite3_column_int(stmt, 2);

                const unsigned char* orderDate = sqlite3_column_text(stmt, 3);
                std::string orderDateStr = orderDate ? std::string(reinterpret_cast<const char*>(orderDate)) : "Unknown";

                const unsigned char* deliveryDate = sqlite3_column_text(stmt, 4);
                std::string deliveryDateStr = deliveryDate ? std::string(reinterpret_cast<const char*>(deliveryDate)) : "null";

                const unsigned char* status = sqlite3_column_text(stmt, 5);
                std::string statusStr = status ? std::string(reinterpret_cast<const char*>(status)) : "Unknown";

                int pickupPointId = sqlite3_column_int(stmt, 6);

                // Форматируем строку таблицы
                char row[256];
                snprintf(row, sizeof(row), "| %-7d | %-6d | %-9d | %-15s | %-14s | %-8s | %-13d |\n",
                    orderId, userId, productId, orderDateStr.c_str(), deliveryDateStr.c_str(), statusStr.c_str(), pickupPointId);
                response += row;
            }

            response += "-------------------------------------------------------------------------------\n";

            sqlite3_finalize(stmt);

            // Если таблица пустая, отправляем соответствующее сообщение
            if (response.size() <= 81) { // Минимальная длина таблицы с заголовками
                sendHttpResponse(clientSocket, "200 OK", "No orders found.");
            }
            else {
                sendHttpResponse(clientSocket, "200 OK", response);
            }
        }
        catch (const std::exception& e) {
            sendHttpResponse(clientSocket, "500 Internal Server Error", "An error occurred while processing the request.");
        }
    }
    
    void addOrder(sqlite3* db, const std::string& customerName, int pickupPointID, const std::vector<int>& productIds) {
        try {
            // Получаем UserID
            int userID = -1;
            std::string getUserSQL = "SELECT UserID FROM Users WHERE Name = ?";
            sqlite3_stmt* stmt;

            if (sqlite3_prepare_v2(db, getUserSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, customerName.c_str(), -1, SQLITE_STATIC);

                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    userID = sqlite3_column_int(stmt, 0);
                }
                sqlite3_finalize(stmt);
            }

            if (userID == -1) {
                throw std::runtime_error("User not found: " + customerName);
            }

            // Добавляем заказ
            for (int productId : productIds) {
                std::string insertOrderSQL =
                    "INSERT INTO Orders (UserID, ProductID, OrderDate, DeliveryDate, Status, PickupPointID) "
                    "VALUES (?, ?, datetime('now'), NULL, 'Pending', ?)";
                if (sqlite3_prepare_v2(db, insertOrderSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_int(stmt, 1, userID);
                    sqlite3_bind_int(stmt, 2, productId);
                    sqlite3_bind_int(stmt, 3, pickupPointID);

                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        throw std::runtime_error("Failed to insert order: " + std::string(sqlite3_errmsg(db)));
                    }

                    sqlite3_finalize(stmt);
                    std::cout << "Order added: UserID=" << userID
                        << ", ProductID=" << productId
                        << ", PickupPointID=" << pickupPointID << std::endl;
                }
            }
        }
        catch (const std::exception& ex) {
            std::cerr << "Error adding order: " << ex.what() << std::endl;
        }
    }


    void handlePlaceOrder(sqlite3* db, int clientSocket, const std::string& body) {
        try {
            // Извлекаем данные из тела запроса
            std::string customerName = extractData(body, "customerName");
            std::string pickupPointIdStr = extractData(body, "pickupPointId");
            std::string productIdStr = extractData(body, "productId");

            // Проверяем корректность данных
            if (customerName.empty() || pickupPointIdStr.empty() || productIdStr.empty()) {
                sendHttpResponse(clientSocket, "400 Bad Request", "Missing required fields: customerName, pickupPointId, or productId.");
                return;
            }

            // Преобразуем идентификаторы из строк в числа
            int pickupPointId = 0, productId = 0;
            try {
                pickupPointId = std::stoi(pickupPointIdStr);
                productId = std::stoi(productIdStr);
            }
            catch (const std::exception&) {
                sendHttpResponse(clientSocket, "400 Bad Request", "Invalid pickupPointId or productId format.");
                return;
            }

            // Вызываем функцию добавления заказа
            try {
                addOrder(db, customerName, pickupPointId, { productId });

                // Отправляем успешный ответ клиенту
                sendHttpResponse(clientSocket, "200 OK", "Order placed successfully.");
            }
            catch (const std::exception& ex) {
                std::cerr << "Error in addOrder: " << ex.what() << std::endl;
                sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to place the order: " + std::string(ex.what()));
            }
        }
        catch (const std::exception& ex) {
            std::cerr << "Error handling placeOrder request: " << ex.what() << std::endl;
            sendHttpResponse(clientSocket, "500 Internal Server Error", "Unexpected error occurred while placing the order.");
        }
    }

    void handleCancelOrder(sqlite3* db, int clientSocket, const std::string& body) {
        // Извлекаем ID заказа из тела запроса
        std::string orderIdStr = extractData(body, "orderId");

        if (orderIdStr.empty()) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Missing orderId parameter");
            return;
        }

        try {
            int orderId = std::stoi(orderIdStr);

            // Создаем SQL-запрос для обновления статуса заказа
            std::string sql = "UPDATE Orders SET Status = 'Cancelled' WHERE OrderID = ?";

            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to prepare SQL statement");
                return;
            }

            // Привязываем значение orderId к запросу
            sqlite3_bind_int(stmt, 1, orderId);

            // Выполняем запрос
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                // Успешное выполнение
                sendHttpResponse(clientSocket, "200 OK", "Order cancelled successfully");
            }
            else {
                // Ошибка выполнения
                sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to cancel the order");
            }

            // Освобождаем ресурсы
            sqlite3_finalize(stmt);
        }
        catch (const std::exception& e) {
            sendHttpResponse(clientSocket, "400 Bad Request", "Invalid orderId format");
        }
    }
    void handleSearchOrder(sqlite3* db, int clientSocket, const std::string& body) {
        try {
            // Извлекаем поле поиска и значение из тела запроса
            std::string searchField = extractData(body, "field");
            std::string searchValue = extractData(body, "value");

            // Проверяем корректность данных
            if (searchField.empty() || searchValue.empty()) {
                sendHttpResponse(clientSocket, "400 Bad Request", "Missing required fields: field or value.");
                return;
            }

            // Разрешенные поля для поиска
            std::unordered_set<std::string> allowedFields = {
                "OrderID", "UserID", "ProductID", "OrderDate", "DeliveryDate", "Status", "PickupPointID"
            };

            if (allowedFields.find(searchField) == allowedFields.end()) {
                sendHttpResponse(clientSocket, "400 Bad Request", "Invalid field for search.");
                return;
            }

            // SQL-запрос для поиска
            std::string sql = "SELECT OrderID, UserID, ProductID, OrderDate, DeliveryDate, Status, PickupPointID FROM Orders WHERE " + searchField + " = ?;";

            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to prepare SQL statement.");
                return;
            }

            // Привязываем значение к запросу
            if (searchField == "OrderID" || searchField == "UserID" || searchField == "ProductID" || searchField == "PickupPointID") {
                sqlite3_bind_int(stmt, 1, std::stoi(searchValue));
            }
            else {
                sqlite3_bind_text(stmt, 1, searchValue.c_str(), -1, SQLITE_STATIC);
            }

            // Формируем табличный ответ
            std::string response;
            response += "-------------------------------------------------------------------------------\n";
            response += "| OrderID | UserID | ProductID | OrderDate       | DeliveryDate   | Status    | PickupPointID |\n";
            response += "-------------------------------------------------------------------------------\n";

            bool found = false;

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                found = true;

                int orderId = sqlite3_column_int(stmt, 0);
                int userId = sqlite3_column_int(stmt, 1);
                int productId = sqlite3_column_int(stmt, 2);

                const unsigned char* orderDate = sqlite3_column_text(stmt, 3);
                std::string orderDateStr = orderDate ? std::string(reinterpret_cast<const char*>(orderDate)) : "Unknown";

                const unsigned char* deliveryDate = sqlite3_column_text(stmt, 4);
                std::string deliveryDateStr = deliveryDate ? std::string(reinterpret_cast<const char*>(deliveryDate)) : "null";

                const unsigned char* status = sqlite3_column_text(stmt, 5);
                std::string statusStr = status ? std::string(reinterpret_cast<const char*>(status)) : "Unknown";

                int pickupPointId = sqlite3_column_int(stmt, 6);

                // Форматируем строку таблицы
                char row[256];
                snprintf(row, sizeof(row), "| %-7d | %-6d | %-9d | %-15s | %-14s | %-8s | %-13d |\n",
                    orderId, userId, productId, orderDateStr.c_str(), deliveryDateStr.c_str(), statusStr.c_str(), pickupPointId);
                response += row;
            }

            response += "-------------------------------------------------------------------------------\n";
            sqlite3_finalize(stmt);

            if (!found) {
                sendHttpResponse(clientSocket, "200 OK", "No orders match the given criteria.");
            }
            else {
                sendHttpResponse(clientSocket, "200 OK", response);
            }
        }
        catch (const std::exception& e) {
            sendHttpResponse(clientSocket, "500 Internal Server Error", "An error occurred while processing the request.");
        }
    }


    void initializeProductsTable(sqlite3* db) {
        char* errorMessage = nullptr;

        // Открываем (или создаём) базу данных
        if (sqlite3_open("sql_db.db", &db) != SQLITE_OK) {
            std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Проверяем, есть ли записи в таблице
        const char* checkTableSQL = "SELECT COUNT(*) FROM Products;";
        sqlite3_stmt* stmt;
        int count = 0;

        if (sqlite3_prepare_v2(db, checkTableSQL, -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                count = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
        else {
            std::cerr << "Error checking table: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        // Если таблица пуста, добавляем данные
        if (count == 0) {
            const char* insertDataSQL = R"(
            INSERT INTO Products (Name, Description, Category, Price)
            VALUES
            ("Smartphone", "A high-end smartphone with 128GB storage", "Electronics", 699.99),
            ("Laptop", "15-inch laptop with 16GB RAM", "Electronics", 999.99),
            ("Headphones", "Wireless noise-cancelling headphones", "Electronics", 199.99),
            ("Refrigerator", "Double-door refrigerator with freezer", "Home Appliances", 799.99),
            ("Washing Machine", "Front-load washing machine with 7kg capacity", "Home Appliances", 499.99),
            ("Microwave", "Digital microwave with 30L capacity", "Home Appliances", 149.99),
            ("Desk Chair", "Ergonomic desk chair with lumbar support", "Furniture", 129.99),
            ("Bookshelf", "5-shelf wooden bookshelf", "Furniture", 79.99),
            ("Tennis Racket", "Professional-grade tennis racket", "Sports", 129.99),
            ("Basketball", "Official size and weight basketball", "Sports", 29.99),
            ("Mountain Bike", "21-speed mountain bike with suspension", "Sports", 499.99),
            ("Electric Guitar", "Electric guitar with amplifier", "Music", 299.99),
            ("Piano", "Digital piano with 88 keys", "Music", 499.99),
            ("Sketchbook", "A3 size sketchbook for artists", "Art Supplies", 19.99),
            ("Paint Set", "24-color acrylic paint set", "Art Supplies", 29.99),
            ("Cookware Set", "10-piece non-stick cookware set", "Kitchen", 99.99),
            ("Knife Set", "6-piece stainless steel knife set", "Kitchen", 49.99),
            ("Vacuum Cleaner", "Bagless vacuum cleaner with HEPA filter", "Home Appliances", 199.99),
            ("Electric Kettle", "1.7L electric kettle with auto shut-off", "Kitchen", 29.99),
            ("Backpack", "Waterproof backpack with multiple compartments", "Accessories", 59.99),
            ("Watch", "Digital watch with heart rate monitor", "Accessories", 149.99),
            ("Sunglasses", "UV-protection polarized sunglasses", "Accessories", 79.99),
            ("Shampoo", "Anti-dandruff shampoo, 500ml", "Personal Care", 9.99),
            ("Soap", "Pack of 3 moisturizing soap bars", "Personal Care", 4.99),
            ("Towel Set", "3-piece cotton towel set", "Home Textiles", 19.99),
            ("Bed Sheets", "Queen-size bed sheet set", "Home Textiles", 39.99),
            ("Garden Hose", "50-foot garden hose with spray nozzle", "Garden", 29.99),
            ("Lawn Mower", "Electric lawn mower with adjustable height", "Garden", 199.99),
            ("Table Lamp", "LED table lamp with adjustable brightness", "Lighting", 49.99),
            ("Ceiling Fan", "3-blade ceiling fan with remote control", "Lighting", 149.99),
            ("Yoga Mat", "Non-slip yoga mat, 6mm thickness", "Fitness", 19.99),
            ("Yoga Block", "Foam yoga block for support", "Fitness", 9.99);
        )";

            if (sqlite3_exec(db, insertDataSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
                std::cerr << "Error inserting data: " << errorMessage << std::endl;
                sqlite3_free(errorMessage);
            }
        }

        // Закрываем базу данных
        sqlite3_close(db);
    }

    void handleGetProducts(sqlite3* db, int clientSocket) {
        try {
            std::string getProductsSQL = "SELECT ProductID, Name, Description, Category, Price FROM Products;";
            sqlite3_stmt* stmt;

            if (sqlite3_prepare_v2(db, getProductsSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to retrieve products.");
                return;
            }

            // Заголовок таблицы
            std::string response = "ProductID   Name                Description         Category       Price\n";
            response += "--------------------------------------------------------------------------\n";

            // Форматируем каждую строку результата
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int productId = sqlite3_column_int(stmt, 0);
                const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                const char* description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                const char* category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                double price = sqlite3_column_double(stmt, 4);

                // Форматируем строку с выравниванием
                char buffer[256];
                snprintf(buffer, sizeof(buffer), "%-11d %-18s %-18s %-13s %.2f\n",
                    productId,
                    name ? name : "",
                    description ? description : "",
                    category ? category : "",
                    price);

                response += buffer;
            }

            sqlite3_finalize(stmt);

            // Отправляем клиенту ответ с таблицей
            sendHttpResponse(clientSocket, "200 OK", response);
        }
        catch (const std::exception& ex) {
            std::cerr << "Error handling getProducts request: " << ex.what() << std::endl;
            sendHttpResponse(clientSocket, "500 Internal Server Error", "Failed to retrieve products.");
        }
    }

}
