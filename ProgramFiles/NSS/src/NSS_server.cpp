#pragma comment(lib, "Ws2_32.lib") // Подключение библиотеки WinSock
#define PORT 8080 // Указываем порт
#define BUFFER_SIZE 1024

#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>
#include "../server/SQL_header.h" // Ваш файл с функциями работы с базой данных
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    setlocale(LC_ALL, "Russian");
    WSADATA wsaData;
    SOCKET serverSocket;
    sqlite3* db;
    const char* dbPath = "sql_db.db";

    // Инициализация WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации WinSock" << std::endl;
        return 1;
    }

    // Создание и настройка сокета
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Ошибка привязки сокета: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Ошибка при попытке слушать: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Открытие базы данных SQLite
    if (sqlite3_open(dbPath, &db) != SQLITE_OK) {
        std::cerr << "Ошибка открытия базы данных: " << sqlite3_errmsg(db) << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Сервер запущен и ожидает подключения на порту " << PORT << "...\n";

    while (true) {
        SOCKET clientSocket;
        struct sockaddr_in clientAddr = {};
        int clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Ошибка принятия соединения: " << WSAGetLastError() << std::endl;
            continue;
        }

        char buffer[BUFFER_SIZE] = { 0 };
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            std::string request(buffer);
            std::cout << "Получен запрос:\n" << request << std::endl;

            // Передача запроса в обработчик
            SQL_Database::handleHttpRequest(db, clientSocket, request);
        }

        closesocket(clientSocket);
    }

    // Закрытие ресурсов
    sqlite3_close(db);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}