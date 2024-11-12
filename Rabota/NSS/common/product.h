#include <string>
#pragma once

class Product {
private:
    int id;                 // ID товара
    std::string name;        // Название товара
    double price;            // Цена товара
    bool isAvailable;        // Статус наличия

public:
    // Конструктор
    Product(int productId, const std::string& productName, double productPrice, bool availability);

    // Геттеры
    int getId() const;
    std::string getName() const;
    double getPrice() const;
    bool getAvailability() const;

    // Сеттеры
    void setId(int productId);
    void setName(const std::string& productName);
    void setPrice(double productPrice);
    void setAvailability(bool availability);

    // Функция для вывода информации о товаре
    void printProductInfo() const; //это скорее всего нужно будет поменять на перегрузку оператора вывода
};