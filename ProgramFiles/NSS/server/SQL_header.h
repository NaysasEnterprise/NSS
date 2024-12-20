#pragma once
#include <string>
#include "sqlite3.h"
#include <winsock2.h>

namespace SQL_Database {

    //для пользователей
    void addUser(sqlite3* db, const std::string& name, const std::string& password);
    bool loginUser(sqlite3* db, const std::string& name, const std::string& password);
    void deleteUser(sqlite3* db, const std::string& name);
    void displayUsers(sqlite3* db);

    void handleGetUserOrders(sqlite3* db, int clientSocket, const std::string& body);

    //вспомогательные функции для обработки пользователей
    void handleAddUser(sqlite3* db, int clientSocket, const std::string& body);
    void handleLoginUser(sqlite3* db, int clientSocket, const std::string& body);
    void handleDeleteUser(sqlite3* db, int clientSocket, const std::string& body);
    void handleDisplayUsers(sqlite3* db, int clientSocket);

    // Выполнение SQL скрипта
    void executeSQLScript(sqlite3* db, const std::string& scriptPath);
    // Обработка HTTP запроса (для различных операций)
    void handleHttpRequest(sqlite3* db, int clientSocket, const std::string& httpRequest);
    // Вспомогательная функция для отправки HTTP ответа
    void sendHttpResponse(int clientSocket, const std::string& status, const std::string& body);
    // Вспомогательная функция для извлечения данных из тела запроса
    std::string extractData(const std::string& body, const std::string& key);
    bool isNumeric(const std::string& str);


    //для пунктов выдачи
    void addPickupPoint(sqlite3* db, const std::string& name, const std::string& address, double coordX, double coordY);
    void editPickupPoint(sqlite3* db, int pickupPointID, const std::string& name, const std::string& address, double coordX, double coordY);
    void deletePickupPoint(sqlite3* db, int pickupPointID);
    void displayPickupPoints(sqlite3* db);

    //вспомогательные для пунктов выдачи
    void handleAddPickupPoint(sqlite3* db, int clientSocket, const std::string& body);
    void handleEditPickupPoint(sqlite3* db, int clientSocket, const std::string& body);
    void handleDeletePickupPoint(sqlite3* db, int clientSocket, const std::string& body);
    void handleDisplayPickupPoints(sqlite3* db, int clientSocket);

}

