#include <iostream>
#include <sqlite3.h>
#include "SQL_header.h"

int main() {
    sqlite3* db;
    const char* dbPath = "your_database.db";  // Укажите здесь путь к вашей базе данных.

    // Открытие базы данных
    int rc = sqlite3_open(dbPath, &db);
    if (rc) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return EXIT_FAILURE;
    }

    // Создание таблицы, если она не существует
    SQL_Database::createTable(db);

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
        case 1: { // Register
            std::string name, password;
            std::cout << "Enter name: ";
            std::cin >> name;
            std::cout << "Enter password: ";
            std::cin >> password;
            SQL_Database::addUser(db, name, password);
            break;
        }
        case 2: { // Login
            std::string name, password;
            std::cout << "Enter name: ";
            std::cin >> name;
            std::cout << "Enter password: ";
            std::cin >> password;
            SQL_Database::loginUser(db, name, password);
            break;
        }
        case 3: { // Delete User
            std::string name;
            std::cout << "Enter name of user to delete: ";
            std::cin >> name;
            SQL_Database::deleteUser(db, name);
            break;
        }
        case 4: { // Display Users
            SQL_Database::displayUsers(db);
            break;
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


