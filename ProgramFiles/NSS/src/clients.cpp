#include "../include/clients.h"

// Добавление заказа
void Client::placeOrder(const std::string& productName) {
    // Создание и добавление заказа в список заказов (для примера добавим заказ с именем продукта)
    Order newOrder(productName); // Примерный конструктор
    orders.push_back(newOrder);
    std::cout << "Заказ на " << productName << " успешно добавлен." << std::endl;
}

// Поиск заказа по ID
void Client::searchOrderByID(int id) {
    for (const auto& order : orders) {
        if (order.getId() == id) { // Предполагается, что у Order есть getId()
            std::cout << "Заказ найден: " << order.get_order_info() << std::endl; 
            return;
        }
    }
    std::cout << "Заказ с ID " << id << " не найден." << std::endl;
}

// Поиск заказа по ФИО
void Client::searchOrderByFullName(const std::string& fullName) {
    for (const auto& order : orders) {
        if (order.getClientName() == fullName) { // Предполагается наличие getClientName()
            std::cout << "Заказ для " << fullName << " найден: " << order.get_order_info() << std::endl;
            return;
        }
    }
    std::cout << "Заказ для " << fullName << " не найден." << std::endl;
}

// Поиск заказа по статусу
void Client::searchOrderByStatus(const std::string& status) {
    for (const auto& order : orders) {
        if (order.getStatus() == status) { // Предполагается наличие getStatus()
            std::cout << "Заказ со статусом " << status << " найден: " << order.get_order_info() << std::endl;
        }
    }
}

// Забор заказа по параметрам
void Client::pickOrderByParameters(const std::string& productName, const std::string& clientName) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (it->getProductName() == productName && it->getClientName() == clientName) {
            receivedOrders.push_back(*it);
            orders.erase(it);
            std::cout << "Заказ на " << productName << " для " << clientName << " забран." << std::endl;
            return;
        }
    }
    std::cout << "Заказ не найден." << std::endl;
}

// Удаление заказа по ID
void Client::removeOrder(int orderId) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (it->getId() == orderId) {
            orders.erase(it);
            std::cout << "Заказ с ID " << orderId << " удален." << std::endl;
            return;
        }
    }
    std::cout << "Заказ с ID " << orderId << " не найден." << std::endl;
}

// Возврат заказа
Order Client::returnOrder(std::string reason) {
    if (!receivedOrders.empty()) {
        Order order = receivedOrders.back();
        receivedOrders.pop_back();
        std::cout << "Возврат заказа: " << order.get_order_info() << " по причине: " << reason << std::endl;
        return order;
    }
    throw std::runtime_error("Нет доступных заказов для возврата.");
}

// Вывод информации о клиенте
void Client::printClientInfo() {
    std::cout << "ID клиента: " << id << std::endl;
    std::cout << "Имя: " << firstName << " " << lastName << std::endl;
    std::cout << "Количество заказов: " << orders.size() << std::endl;
}

