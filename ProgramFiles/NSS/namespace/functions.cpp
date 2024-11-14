#include "../include/order_pickup_point.h"
#include <iostream>
#include <random>  // Для генерации случайных чисел
#include <set>     // Для хранения существующих ID
#include <fstream>

using namespace FunctionsOPPControl;


void FunctionsOPPControl::adminMenu() {
    std::cout << "\n--- Администратор ---\n";
    std::cout << "1. Посмотреть все заказы\n";
    std::cout << "2. Найти заказ по ID\n";
    std::cout << "3. Добавить новый заказ\n";
    std::cout << "4. Создать новый ПВЗ\n";
    std::cout << "5. Показать все ПВЗ\n";
    std::cout << "6. Показать всех клиентов\n";
    std::cout << "7. Вернуться в главное меню\n";
    std::cout << "Выберите опцию: ";
}
void FunctionsOPPControl::clientMenu() {
    std::cout << "\n--- Клиент ---\n";
    std::cout << "1. Посмотреть мои заказы\n";
    std::cout << "2. Оформить новый заказ\n";
    std::cout << "3. Найти заказ по ID\n";
    std::cout << "4. Вернуться в главное меню\n";
}

int FunctionsOPPControl::generateUniquePVZId() {
    std::set<int> existingIds;
    std::ifstream file("pvz_data.txt");

    // Считываем существующие ID из файла и добавляем их в множество
    if (file.is_open()) {
        int id;
        std::string name;
        double x, y;
        while (file >> id >> name >> x >> y) {
            existingIds.insert(id);
        }
        file.close();
    }

    // Генерация нового уникального ID
    int newId;
    do {
        newId = rand() % 10000 + 1;  // Генерация случайного числа от 1 до 10000
    } while (existingIds.find(newId) != existingIds.end());

    return newId;
}