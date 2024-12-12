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

}
