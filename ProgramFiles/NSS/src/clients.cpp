#include "../include/clients.h"
#include <iostream>
#include <algorithm>

// Геттеры для доступа к полям
int Client::getId() const {
    return id;
}

std::string Client::getFirstName() const {
    return firstName;
}

std::string Client::getLastName() const {
    return lastName;
}

std::vector<Order> Client::getOrders() const {
    return orders;
}

// Сеттеры для изменения полей
void Client::setFirstName(const std::string& fName) {
    firstName = fName;
}

void Client::setLastName(const std::string& lName) {
    lastName = lName;
}

void Client::setId(int clientId) {
    id = clientId;
}

// Функция для оформления заказа по названию товара
void Client::placeOrder(const std::string& productName) {
    Order newOrder(productName);
    orders.push_back(newOrder);
    std::cout << "Order placed: " << productName << std::endl;
}

// Функция для поиска заказа по ID
void Client::searchOrderByID(int orderId) {
    auto it = std::find_if(orders.begin(), orders.end(),
        [orderId](const Order& order) { return order.getOrderId() == orderId; });
    if (it != orders.end()) {
        std::cout << "Order found: " << it->getProductName() << std::endl;
    }
    else {
        std::cout << "Order with ID " << orderId << " not found." << std::endl;
    }
}

// Функция для поиска заказа по полному имени клиента
void Client::searchOrderByFullName(const std::string& fullName) {
    auto it = std::find_if(orders.begin(), orders.end(),
        [fullName](const Order& order) { return order.getClientName() == fullName; });
    if (it != orders.end()) {
        std::cout << "Order found for " << fullName << ": " << it->getProductName() << std::endl;
    }
    else {
        std::cout << "Order for " << fullName << " not found." << std::endl;
    }
}

// Функция для поиска заказа по статусу
void Client::searchOrderByStatus(const std::string& status) {
    auto it = std::find_if(orders.begin(), orders.end(),
        [status](const Order& order) { return order.getStatus() == status; });
    if (it != orders.end()) {
        std::cout << "Order with status " << status << ": " << it->getProductName() << std::endl;
    }
    else {
        std::cout << "Order with status " << status << " not found." << std::endl;
    }
}

// Функция для забора заказа по параметрам
void Client::pickOrderByParameters(const std::string& productName, const std::string& clientName) {
    auto it = std::find_if(orders.begin(), orders.end(),
        [productName, clientName](const Order& order) {
            return order.getProductName() == productName && order.getClientName() == clientName;
        });
    if (it != orders.end()) {
        receivedOrders.push_back(*it);
        orders.erase(it);
        std::cout << "Order picked up: " << productName << " by " << clientName << std::endl;
    }
    else {
        std::cout << "No matching order found." << std::endl;
    }
}

// Удаление заказа по ID
void Client::removeOrder(int orderId) {
    auto it = std::remove_if(orders.begin(), orders.end(),
        [orderId](const Order& order) { return order.getOrderId() == orderId; });
    if (it != orders.end()) {
        orders.erase(it, orders.end());
        std::cout << "Order with ID " << orderId << " removed." << std::endl;
    }
    else {
        std::cout << "Order with ID " << orderId << " not found." << std::endl;
    }
}

// Функция для возврата заказа по причине
Order Client::returnOrder(std::string reason) {
    if (orders.empty()) {
        std::cout << "No orders available for return." << std::endl;
        return Order(""); // Вернуть пустой заказ, если список пуст
    }

    std::cout << "Available orders for return:" << std::endl;
    for (size_t i = 0; i < orders.size(); ++i) {
        std::cout << i + 1 << ". " << orders[i].getProductName() << " (ID: " << orders[i].getOrderId() << ")" << std::endl;
    }

    int choice;
    std::cout << "Enter the number of the order to return: ";
    std::cin >> choice;

    if (choice < 1 || choice > static_cast<int>(orders.size())) {
        std::cout << "Invalid choice. No order returned." << std::endl;
        return Order(""); // Вернуть пустой заказ, если выбор некорректен
    }

    Order returnedOrder = orders[choice - 1];
    orders.erase(orders.begin() + (choice - 1)); // Удалить заказ из списка

    std::cout << "Order with ID " << returnedOrder.getOrderId()
        << " returned due to: " << reason << std::endl;

    // Здесь можно сохранить информацию о возвращённых заказах в отдельный контейнер, если нужно
    // Например: returnedOrders.push_back(returnedOrder);

    return returnedOrder; // Вернуть заказ
}


// Функция для вывода информации о клиенте
void Client::printClientInfo() {
    std::cout << "Client ID: " << id << "\n"
        << "Name: " << firstName << " " << lastName << std::endl;
}

// Функция для генерации уникального ID клиента
int Client::generateUniqueClientId() {
    static int idCounter = 1;
    return idCounter++;
}
