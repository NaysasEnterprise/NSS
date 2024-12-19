#pragma once
#include <string>
#include "sqlite3.h"
#include <winsock2.h>

namespace SQL_Database {

    // Добавление нового пользователя в базу данных
    void addUser(sqlite3* db, const std::string& name, const std::string& password);

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

    // Вспомогательная функция для извлечения данных из тела запроса
    std::string extractData(const std::string& body, const std::string& key);
}
