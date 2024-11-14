#pragma once
#include <string>
#include "product.h"

class Order {
private:
    int order_id;
    Product product;           // Продукт, связанный с заказом
    std::string client_name;
    std::string order_date;
    std::string status;
    int quantity;
    bool is_ready;

public:
    // Конструктор заказа
    Order(int orderId, const Product& prod, const std::string& client, const std::string& date,
        const std::string& orderStatus, int qty);

    // Функции для работы с заказом
    void updateStatus(const std::string& newStatus);
    bool isOrderReady() const;
    int getQuantity() const;
    std::string getOrderInfo() const;

    // Геттеры
    int getId() const;
    std::string getProductName() const;
    std::string getClientName() const;
    std::string getStatus() const;
};
