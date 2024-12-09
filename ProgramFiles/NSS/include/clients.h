#include <string>
#include <vector>
#include "order.h"
#include <iostream>
#pragma once

class Client {
private:
    int id;
    std::string firstName;
    std::string lastName;
    std::vector<Order> orders;
    std::vector<Order> receivedOrders;
public:

    Client(int clientId, const std::string& firstName, const std::string& lastName)
        : id(clientId), firstName(firstName), lastName(lastName) {}

    // Геттеры для доступа к полям
    int getClientId() const;
    std::string getClientFirstName() const;
    std::string getClientLastName() const;
    std::vector<Order> getClientOrders() const;

    // Сеттеры для изменения полей
    void setClientFirstName(const std::string& fName);
    void setClientLastName(const std::string& lName);
    void setClientId(int clientId);

    // Заголовок функции для оформления заказа по названию товара
    void placeOrder(const std::string& productName);

    // Заголовок функции для поиска заказа по параметрам (ID, ФИО, статус заказа)
    void searchOrder(int id);

    // Заголовок функции для забора заказа (поиск товара по параметрам)
    void pickOrderByParameters(const std::string& productName, const std::string& clientName);

    void removeOrder(int orderId);
    // Функция для вывода информации о клиенте

    Order returnOrder(std::string reason);

    void printClientInfo() const;

    int generateUniqueClientId();
    
    int main();
};