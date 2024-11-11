// product.cpp
#include "product.h"
#include <iostream>

// Конструктор
Product::Product(int productId, const std::string& productName, double productPrice, bool availability)
    : id(productId), name(productName), price(productPrice), isAvailable(availability) {} //TODO: Реализовать конструктор

// Геттеры
int Product::getId() const { return id; }
std::string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
bool Product::getAvailability() const { return isAvailable; }

// Сеттеры
void Product::setId(int productId) { id = productId; }
void Product::setName(const std::string& productName) { name = productName; }
void Product::setPrice(double productPrice) { price = productPrice; }
void Product::setAvailability(bool availability) { isAvailable = availability; }

// Функция для вывода информации о товаре
void Product::printProductInfo() const {
    std::cout << "ID товара: " << id << std::endl;
    std::cout << "Название: " << name << std::endl;
    std::cout << "Цена: " << price << std::endl;
    std::cout << "Наличие: " << (isAvailable ? "Да" : "Нет") << std::endl;
} //тут еще надо продумать как у нас будет работать с клиентом(будут скорее всего заводиться "аккаунты" клиентов
//TODO: Продумать работу с клиентом
//TODO: Переписать на перегрузку вывода