#include "order.h"
#include <sstream>

// Конструктор
Order::Order(int orderId, const Product& prod, const std::string& client, const std::string& date,
    const std::string& orderStatus, int qty)
    : order_id(orderId), product(prod), client_name(client), order_date(date),
    status(orderStatus), quantity(qty), is_ready(false) {}

// Обновление статуса заказа
void Order::updateStatus(const std::string& newStatus) {
    status = newStatus;
    is_ready = (status == "Готов");
}

// Проверка готовности заказа
bool Order::isOrderReady() const {
    return is_ready;
}

// Получение количества товаров
int Order::getQuantity() const {
    return quantity;
}

// Формирование информации о заказе
std::string Order::getOrderInfo() const {
    std::ostringstream oss;
    oss << "ID Заказа: " << order_id << "\n"
        << "Продукт: " << product.getName() << "\n"
        << "Клиент: " << client_name << "\n"
        << "Дата заказа: " << order_date << "\n"
        << "Статус: " << status << "\n"
        << "Количество: " << quantity << "\n"
        << "Готовность: " << (is_ready ? "Да" : "Нет") << "\n";
    return oss.str();
}

// Геттеры
int Order::getId() const {
    return order_id;
}
std::string Order::getProductName() const {
    return product.getName();
}
std::string Order::getClientName() const {
    return client_name;
}
std::string Order::getStatus() const {
    return status;
<<<<<<< HEAD
}
=======
}p
>>>>>>> d127afe88b3b6eadcb1cfeeab7479238ed605cc9
