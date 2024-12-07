﻿// NSS.cpp: определяет точку входа для приложения.
#include <iostream>
#include <winsock2.h> // Основной заголовок для WinSock
#include <ws2tcpip.h> // Для дополнительных функций сокетов (например, getaddrinfo)
#include <string>
#include <vector>
#include "../include/order.h"
#include "../include/order_pickup_point.h"
#include "../include/product.h"
#include "../include/clients.h"


using namespace FunctionsOPPControl;


#pragma comment(lib, "Ws2_32.lib") // Подключение библиотеки WinSock
#define PORT 8080 // Указываем порт

int main() {
    system("chcp 1251");
    setlocale(LC_ALL, "Russian");  // Установка локали для поддержки русского языка

    int choice{};

    while (true) {
        // Главное меню
        startMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: {
            // Меню для администратора
            int adminChoice;
            while (true) {
                FunctionsOPPControl::adminMenu();
                std::cin >> adminChoice;

                switch (adminChoice) {
                case 1:
                    // Вызов функции просмотра всех заказов
                    
                    break;
                case 2:
                    // Вызов функции поиска заказа по ID
                    std::cout << "Введите ID заказа для поиска: ";
                    int orderId;
                    std::cin >> orderId;
                     // Поиск заказа по ID
                    
                    break;
                case 3:
                    // Вызов функции добавления нового заказа

                    break;
                case 4:
                    adminCreateOPP();  // Создание нового ПВЗ
                    break;
                case 5:
                    showAllOPP();      // Показ всех ПВЗ
                    break;
                case 6:
                    showAllClients();  // Показ всех клиентов
                    break;
                case 7:
                    std::cout << "Возврат в главное меню.\n";
                    goto mainMenu;  // Возврат в главное меню
                default:
                    std::cout << "Неверный выбор. Пожалуйста, попробуйте снова.\n";
                }
            }
            break;
        }

        case 2: {
            clientLoginOrRegister(); // Вход или регистрация клиента

            int clientChoice;
            while (true) {
                clientMenu();
                std::cout << "Введите ваш выбор: ";
                std::cin >> clientChoice;

                switch (clientChoice) {
                case 1:
                    std::cout << "Показать мои заказы (здесь будет логика)\n";
                    break;
                case 2:
                    std::cout << "Оформление нового заказа (здесь будет логика)\n";
                    
                    break;
                case 3:
                    std::cout << "Введите ID заказа для поиска: ";
                    // Логика для поиска заказа по ID
                    break;
                case 4:
                    std::cout << "Возвращение в главное меню...\n";
                    goto mainMenu;  // Возврат в главное меню
                default:
                    std::cout << "Некорректный выбор. Попробуйте снова.\n";
                }
            }
            break;
        }

        case 3:
            std::cout << "Выход из программы...\n";
            return 0;  // Завершение программы

        default:
            std::cout << "Некорректный выбор. Попробуйте снова.\n";
        }

    mainMenu:;  // Метка для возврата в главное меню
    }

    return 0;
}
#define BUFFER_SIZE 1024

/*int main() {
    setlocale(LC_ALL, "Russian");
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = { 0 };
    const char* response = "fkfkfkkfkf";

    // Инициализация WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации WinSock: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Создание серверного сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Настройка сокета (SO_REUSEADDR для повторного использования адреса)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "Ошибка установки параметров сокета: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Привязка сокета к адресу
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Ошибка привязки сокета: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Ошибка прослушивания: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Ожидание подключения..." << std::endl;

    // Принятие нового подключения
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket == INVALID_SOCKET) {
        std::cerr << "Ошибка принятия подключения: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Клиент подключен!" << std::endl;

    // Получение запроса от клиента
    int bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_read > 0) {
        std::cout << "Запрос от клиента: " << buffer << std::endl;

        // Отправка ответа клиенту
        send(new_socket, response, strlen(response), 0);
        std::cout << "Ответ отправлен: " << response << std::endl;
    }

    // Закрытие сокетов
    closesocket(new_socket);
    closesocket(server_fd);

    // Очистка WinSock
    WSACleanup();

    return 0;
}*/
