#include <string>
#include <vector>
#include "order.h"
#include <iostream>
#pragma once

class OPP {
private:
    struct Client {
        std::string firstName;
        std::string lastName;
        std::vector<Order> orders;
    };

    std::vector<Client> clients; // Список клиентов
    std::vector<Order> inventory; // Заказы, находящиеся на ПВЗ

    int id;
    std::string name;
    double coordX;
    double coordY;

public:
    // Конструктор
    OPP(int id, const std::string& name, double coordX, double coordY);

    // Геттеры
    int getId() const;
    std::string getName() const;
    double getCoordX() const;
    double getCoordY() const;

    // Добавление клиента
    void addClient(const std::string& firstName, const std::string& lastName);

    // Проверка наличия клиента
    bool clientExists(const std::string& firstName, const std::string& lastName) const;

    // Добавление заказа в инвентарь ПВЗ
    void addOrderToInventory(const Order& order);

    // Получение всех заказов клиента
    std::vector<Order> getClientOrders(const std::string& firstName, const std::string& lastName) const;

    // Удаление заказа после получения
    bool removeOrder(const int& orderID);

    // Вывод информации о всех клиентах
    void printClients() const;

    // Вывод информации о всех заказах в ПВЗ
    void printInventory() const;

    void printOPPInfo() const;
};

namespace FunctionsOPPControl {
    void adminMenu();
    void clientMenu();
    void startMenu();
    void searchMenu();
    bool clientExists(const std::string& firstName, const std::string& lastName);
    void addClient(const std::string& firstName, const std::string& lastName);
    void clientLoginOrRegister();

    int generateUniqueClientId();
    void addClient(const std::string& firstName, const std::string& lastName);
    bool clientExists(const std::string& firstName, const std::string& lastName);
    void showAllClients();

    int generateUniqueOPPId();
    void adminCreateOPP();      // Создание ПВЗ администратором
    void showAllOPP();          // Просмотр всех ПВЗ
    void addOPPToFile(const OPP& opp);
}
