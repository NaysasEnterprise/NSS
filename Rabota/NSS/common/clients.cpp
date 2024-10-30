#include "clients.h"

// Геттеры
int Client::getId() const { return id; }
std::string Client::getFirstName() const { return firstName; }
std::string Client::getLastName() const { return lastName; }
std::string Client::getMiddleName() const { return middleName; }
std::vector<Order> Client::getOrders() const { return orders; }

// Сеттеры
void Client::setFirstName(const std::string& fName) { firstName = fName; }
void Client::setLastName(const std::string& lName) { lastName = lName; }
void Client::setMiddleName(const std::string& mName) { middleName = mName; }
void Client::setId(int clientId) { id = clientId; }

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in server_address;
    const char* request = "Сделать заказ";
    char buffer[BUFFER_SIZE] = { 0 };

    // Инициализация WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации WinSock: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Подключение к серверу
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        std::cerr << "Ошибка подключения к серверу: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Отправка запроса серверу
    send(sock, request, strlen(request), 0);
    std::cout << "Запрос отправлен: " << request << std::endl;

    // Получение ответа от сервера
    int bytes_read = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_read > 0) {
        std::cout << "Ответ от сервера: " << buffer << std::endl;
    }

    // Закрытие сокета
    closesocket(sock);

    // Очистка WinSock
    WSACleanup();

    return 0;
}
