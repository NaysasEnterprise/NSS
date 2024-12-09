#include <iostream>
#include "sqlite3.h"
#include "SQL_header.h"

int main() {
    sqlite3* db;
    const char* dbPath = "sql_db.db";
    const char* sqlScriptPath = "sql_script.sql"; // Укажите путь к вашему SQL-скрипту

    // Открытие базы данных
    if (sqlite3_open(dbPath, &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return EXIT_FAILURE;
    }

    // Выполнение SQL-скрипта
    SQL_Database::executeSQLScript(db, sqlScriptPath);

    // Проверка наличия таблицы
    const char* checkTableSQL = "SELECT name FROM sqlite_master WHERE type='table' AND name='Users';";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, checkTableSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "Table 'Users' exists." << std::endl;
        }
        else {
            std::cerr << "Table 'Users' does not exist!" << std::endl;
        }
        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Failed to check table: " << sqlite3_errmsg(db) << std::endl;
    }

    // Основное меню
    int choice;
    do {
        std::cout << "Choose an option:\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. Delete User\n";
        std::cout << "4. Display Users\n";
        std::cout << "0. Exit\n";
        std::cout << "Your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string name, password;
            std::cout << "Enter name: ";
            std::cin >> name;
            std::cout << "Enter password: ";
            std::cin >> password;
            SQL_Database::addUser(db, name, password);
            break;
        }
        case 2: {
            std::string name, password;
            std::cout << "Enter name: ";
            std::cin >> name;
            std::cout << "Enter password: ";
            std::cin >> password;
            SQL_Database::loginUser(db, name, password);
            break;
        }
        case 3: {
            std::string name;
            std::cout << "Enter name of user to delete: ";
            std::cin >> name;
            SQL_Database::deleteUser(db, name);
            break;
        }
        case 4:
            SQL_Database::displayUsers(db);
            break;
        case 0:
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


