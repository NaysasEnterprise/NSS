#include "../include/clients.h"

// Конструктор заказа
Order::Order(int order_id, const std::string& product, const std::string& client,
    const std::string& date, const std::string& order_status, int qty)
    : order_id(order_id), product_name(product), client_name(client),
    order_date(date), status(order_status), quantity(qty)
{
    is_ready = (status == "Готов"); // Инициализация готовности в зависимости от статуса
}

// Функция для изменения статуса заказа
void Order::update_status(const std::string& new_status) {
    status = new_status;
    is_ready = (status == "Готов"); // Обновляем флаг готовности, если статус "Готов"
}

// Функция для проверки готовности заказа
bool Order::is_order_ready() const {
    return is_ready;
}

// Функция для проверки количества товаров в заказе
int Order::get_quantity() const {
    return quantity;
}

// Получить данные о заказе в формате строки
std::string Order::get_order_info() const {
    return "ID заказа: " + std::to_string(order_id) + "\n" +
        "Продукт: " + product_name + "\n" +
        "Клиент: " + client_name + "\n" +
        "Дата заказа: " + order_date + "\n" +
        "Статус: " + status + "\n" +
        "Количество: " + std::to_string(quantity) + "\n" +
        "Готовность: " + (is_ready ? "Да" : "Нет") + "\n";
}

// Недостающие геттеры

int Order::getId() const {
    return order_id;
}

std::string Order::getProductName() const {
    return product_name;
}

std::string Order::getClientName() const {
    return client_name;
}

std::string Order::getStatus() const {
    return status;
}p