#include <string>
#include <vector>
#include "order.h"
#include <iostream>
#include <fstream>
#pragma once

class Client {
private:
    int id;
    int opp_id;
    std::string firstName;
    std::string lastName;
    std::vector<Order> orders;
    std::vector<Order> receivedOrders;
public:

    Client(int clientId, const std::string& firstName, const std::string& lastName)
        : id(clientId), firstName(firstName), lastName(lastName) {}
    // Геттеры для доступа к полям
    int getId() const;
    int getOPPId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::vector<Order> getOrders() const;

    // Сеттеры для изменения полей
    void setFirstName(const std::string& fName);
    void setLastName(const std::string& lName);
    int setOPPId(int oppId);
    void setId(int clientId);

    // Заголовок функции для оформления заказа по названию товара
    void placeOrder(const Product& productName);

    // Заголовок функции для поиска заказа по параметрам (ID, ФИО, статус заказа)
    void searchOrderByID(int id);
    void searchOrderByStatus(const std::string& status);

    // Заголовок функции для забора заказа (поиск товара по параметрам)
    void pickOrderByParameters(const std::string& productName, const std::string& clientName);

    // Функция для вывода информации о клиенте

    Order returnOrder(std::string reason);

    void printClientInfo();
    void printOrders();

    int generateUniqueClientId();
};