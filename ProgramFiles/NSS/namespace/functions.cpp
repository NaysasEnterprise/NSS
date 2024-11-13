#include "../include/order_pickup_point.h"
#include <iostream>

using namespace FunctionsOPPControl;


void FunctionsOPPControl::adminMenu() {
    std::cout << "\n--- Администратор (сотрудник ПВЗ) ---\n";
    std::cout << "1. Посмотреть все заказы\n";
    std::cout << "2. Найти заказ по ID\n";
    std::cout << "3. Добавить новый заказ\n";
    std::cout << "4. Вернуться в главное меню\n";
}
void FunctionsOPPControl::clientMenu() {
    std::cout << "\n--- Клиент ---\n";
    std::cout << "1. Посмотреть мои заказы\n";
    std::cout << "2. Оформить новый заказ\n";
    std::cout << "3. Найти заказ по ID\n";
    std::cout << "4. Вернуться в главное меню\n";
}