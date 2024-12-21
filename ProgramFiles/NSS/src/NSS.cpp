// NSS.cpp: определяет точку входа для приложения.
#include <iostream>
#include <string>
#include <vector>
#include "../include/order.h"
#include "../include/order_pickup_point.h"
#include "../include/product.h"
#include "../include/clients.h"

using namespace FunctionsOPPControl;

int main() {//
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