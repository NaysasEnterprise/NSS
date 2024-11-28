#include "../include/product.h"
#include <random>

using namespace std;

// Конструктор
Product::Product(int productId, const std::string& productName, double productPrice, bool availability)
    : id(productId), name(productName), price(productPrice), isAvailable(availability) {}

// Геттеры
int Product::getId() const { return id; }
string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
bool Product::getAvailability() const { return isAvailable; }

// Сеттеры
void Product::setId(int productId) { id = productId; }
void Product::setName(const std::string& productName) { name = productName; }
void Product::setPrice(double productPrice) { price = productPrice; }
void Product::setAvailability(bool availability) { isAvailable = availability; }

// Перегрузка оператора вывода
ostream& operator<<(ostream& os, const Product& product) {
    os << "ID товара: " << product.id << "\n"
        << "Название: " << product.name << "\n"
        << "Цена: " << product.price << "\n"
        << "Наличие: " << (product.isAvailable ? "Да" : "Нет") << "\n";
    return os;
}

// Генерация списка продуктов
vector<Product> Product::generateProducts(int count) {
    vector<Product> products;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> priceDistrib(10.0, 500.0);
    uniform_int_distribution<> availabilityDistrib(0, 1);

    for (int i = 1; i <= count; ++i) {
        double price = priceDistrib(gen);
        bool availability = availabilityDistrib(gen);
        std::string productName = "Product" + std::to_string(i);
        products.emplace_back(i, productName, price, availability);
    }

    return products;
}