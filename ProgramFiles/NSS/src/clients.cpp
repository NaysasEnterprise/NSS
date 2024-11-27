#include "../include/clients.h"
#include "../include/order_pickup_point.h"
#include "../include/order.h"
#include <iostream>
#include <algorithm>

using namespace FunctionsOPPControl;
using namespace std;

// Геттеры для доступа к полям
int Client::getClientId() const {
    return id;
}

std::string Client::getClientFirstName() const {
    return firstName;
}

std::string Client::getClientLastName() const {
    return lastName;
}

std::vector<Order> Client::getClientOrders() const {
    return orders;
}


// Сеттеры для изменения полей
void Client::setClientFirstName(const std::string& fName) {
    firstName = fName;
}

void Client::setClientLastName(const std::string& lName) {
    lastName = lName;
}

void Client::setClientId(int clientId) {
    id = clientId;
}


// Функция для оформления заказа по названию товара
void Client::placeOrder(const std::string& productName) {
    //Order newOrder(productName);
    //orders.push_back(newOrder);
    std::cout << "Order placed: " << productName << std::endl;
}

// Функция для поиска заказа по ID
void Client::searchOrder(int orderId) {
    int key{};

    // вызов меню для поиска товаров
    searchMenu();
    cin >> key;

    // Объявление итератора до switch, чтобы он был доступен в каждом случае
    std::vector<Order>::iterator it;

    switch (key)
    {
    case 1:
        std::cout << "Enter Order ID: ";
        std::cin >> orderId;

        it = std::find_if(orders.begin(), orders.end(),
            [orderId](const Order& order) { return order.getOrderId() == orderId; });

        if (it != orders.end()) {
            std::cout << "Order found: " << it->getProductName() << std::endl;
        }
        else {
            std::cout << "Order with ID " << orderId << " not found." << std::endl;
        }
        break;

    case 2: { // Поиск по полному имени клиента
        std::cout << "Enter Client Full Name: ";
        std::string fullName;
        std::cin.ignore();
        std::getline(std::cin, fullName);

        it = std::find_if(orders.begin(), orders.end(),
            [fullName](const Order& order) { return order.getClientName() == fullName; });

        if (it != orders.end()) {
            std::cout << "Order found for " << fullName << ": " << it->getProductName() << std::endl;
        }
        else {
            std::cout << "Order for " << fullName << " not found." << std::endl;
        }
        break;
    }
    case 3: { // Поиск по статусу заказа
        std::cout << "Enter Order Status: ";
        std::string status;
        std::cin.ignore();
        std::getline(std::cin, status);

        it = std::find_if(orders.begin(), orders.end(),
            [status](const Order& order) { return order.getStatus() == status; });

        if (it != orders.end()) {
            std::cout << "Order with status " << status << ": " << it->getProductName() << std::endl;
        }
        else {
            std::cout << "Order with status " << status << " not found." << std::endl;
        }
        break;
    }
    case 4: // Выход
        std::cout << "Exiting search menu." << std::endl;
        return;
    default:
        std::cout << "Invalid option. Please try again." << std::endl;
        break;
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
        return Order(0, Product(0, "", 0.0, false), "", "", "", 0); // Вернуть пустой заказ с нулевыми значениями
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
        return Order(0, Product(0, "", 0.0, false), "", "", "", 0); // Вернуть пустой заказ, если выбор некорректен
    }

    Order returnedOrder = orders[choice - 1];
    orders.erase(orders.begin() + (choice - 1)); // Удалить заказ из списка

    std::cout << "Order with ID " << returnedOrder.getOrderId()
        << " returned due to: " << reason << std::endl;

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
