#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // Для функции inet_pton
#pragma comment(lib, "Ws2_32.lib") // Подключение библиотеки WinSock
#define PORT 8080 // Указываем порт для подключения

int main() {
    WSADATA wsaData;

    // Инициализация WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации WinSock" << std::endl;
        return 1;
    }

    SOCKET clientSocket;
    struct sockaddr_in serverAddr;

    // Создание клиентского сокета
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // Укажите IP-адрес сервера (например, 127.0.0.1 для локального подключения)
    const char* serverIP = "127.0.0.1";
    if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Неверный адрес или адрес не поддерживается" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Подключение к серверу
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка подключения: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Подключение к серверу успешно!" << std::endl;

    // Закрытие сокета после завершения работы
    closesocket(clientSocket);

    // Очистка WinSock
    WSACleanup();
    return 0;
}