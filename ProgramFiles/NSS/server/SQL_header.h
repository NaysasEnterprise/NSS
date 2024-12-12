#pragma once
#include <string>
#include "sqlite3.h"
#include <winsock2.h>

namespace SQL_Database {
    void createTable(sqlite3* db) {
        const char* sqlCreateTable =
            "CREATE TABLE IF NOT EXISTS Users ("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "Name TEXT NOT NULL UNIQUE,"
            "Password TEXT NOT NULL);";

<<<<<<< HEAD
        char* errorMessage = NULL;  // Заменяем nullptr на NULL
        if (sqlite3_exec(db, sqlCreateTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
            std::cerr << "SQL error: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
    }
=======
    // Добавление нового пользователя в базу данных
    void addUser(sqlite3* db, const std::string& name, const std::string& password);
>>>>>>> 672a63b6dbef354c6c5b0da9130ae765fca43d6c

    // Вход пользователя с проверкой имени и пароля
    bool loginUser(sqlite3* db, const std::string& name, const std::string& password);

    // Удаление пользователя по имени
    void deleteUser(sqlite3* db, const std::string& name);

    // Отображение всех пользователей
    void displayUsers(sqlite3* db);

    // Выполнение SQL скрипта
    void executeSQLScript(sqlite3* db, const std::string& scriptPath);

    // Обработка HTTP запроса (для различных операций)
    void handleHttpRequest(sqlite3* db, int clientSocket, const std::string& httpRequest);

    // Вспомогательные функции для обработки пользователей
    void handleAddUser(sqlite3* db, int clientSocket, const std::string& body);
    void handleLoginUser(sqlite3* db, int clientSocket, const std::string& body);
    void handleDeleteUser(sqlite3* db, int clientSocket, const std::string& body);
    void handleDisplayUsers(sqlite3* db, int clientSocket);

    // Вспомогательная функция для отправки HTTP ответа
    void sendHttpResponse(int clientSocket, const std::string& status, const std::string& body);

<<<<<<< HEAD
        // Привязка параметра
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

        // Исполнение запроса
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

        // Подготовка SQL запроса
        if (sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Привязка параметра
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

        // Выполнение запроса
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "User deleted successfully!" << std::endl;
        }

        // Освобождение ресурсов
        sqlite3_finalize(stmt);
    }

    void displayUsers(sqlite3* db) {
        const char* sqlSelect = "SELECT * FROM Users;";
        sqlite3_stmt* stmt;

        // Подготовка SQL запроса
        if (sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        std::cout << "Users in the database:\n";
        std::cout << "ID\tName\tPassword\n";
        std::cout << "----------------------------------\n";

        // Обработка результата
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            std::cout << id << "\t" << (name ? name : "NULL") << "\t" << (password ? password : "NULL") << "\n";
        }

        // Освобождение ресурсов
        sqlite3_finalize(stmt);
    }
} // namespace SQL_Database

    void executeSQLScript(sqlite3* db, const std::string& scriptPath) {
        std::ifstream scriptFile(scriptPath);
        if (!scriptFile.is_open()) {
            std::cerr << "Cannot open SQL script file: " << scriptPath << std::endl;
            return;
        }

        std::stringstream buffer;
        buffer << scriptFile.rdbuf();
        std::string sql = buffer.str();

        char* errorMessage = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
            std::cerr << "SQL error: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
        else {
            std::cout << "SQL script executed successfully!" << std::endl;
        }
        case 0: // Exit
            std::cout << "Exiting...\n";
            break;
        default:
            std::cerr << "Invalid choice! Please try again.\n";
    }
    } while (choice != 0);

    // Закрытие базы данных
    sqlite3_close(db);
    return 0;
}
=======
    // Вспомогательная функция для извлечения данных из тела запроса
    std::string extractData(const std::string& body, const std::string& key);
}
>>>>>>> 672a63b6dbef354c6c5b0da9130ae765fca43d6c
