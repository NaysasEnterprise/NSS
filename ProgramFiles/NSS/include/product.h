#pragma once
#include <string>
#include <iostream>
#include <vector>

class Product {
private:
    int id;                   // ID товара
    std::string name;         // Название товара
    double price;             // Цена товара
    bool isAvailable;         // Статус наличия

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

    // Перегрузка оператора вывода для класса Product
    friend std::ostream& operator<<(std::ostream& os, const Product& product);

    // Генерация списка продуктов
    static std::vector<Product> generateProducts(int count);
};