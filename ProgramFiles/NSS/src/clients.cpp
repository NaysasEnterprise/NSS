#include "../include/clients.h"
#include "../include/order_pickup_point.h"
#include "../include/order.h"
#include <iostream>
#include <algorithm>

using namespace FunctionsOPPControl;
using namespace std;
#define COMMON_ORDERS_FILE "all_orders.txt"

Client::Client(int clientId, const std::string& firstName, const std::string& lastName)
    : id(clientId), firstName(firstName), lastName(lastName) {}

int Client::getId() const{
    return id;
}
int Client::getOPPId() const{
    return opp_id;
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

void Client::setFirstName(const std::string& fName) {
    firstName = fName;
}
void Client::setLastName(const std::string& lName) {
    lastName = lName;
}
void Client::setId(int clientId) {
    id = clientId;
}
int Client::setOPPId(int oppId) {
    opp_id = oppId;
}

// Добавление заказа
void Client::placeOrder(const Product& produc, int& qty) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    Order newOrder(generateUniqueClientId,product,firstName+" "+lastName, std::put_time(&tm, "%d-%m-%Y %H:%M:%S"), "Сборка", qty);
    orders.push_back(newOrder);
    std::cout << "Заказ на " << productName << " успешно добавлен." << std::endl;

    std::ofstream file(COMMON_ORDERS_FILE, std::ios::app);
    if (file.is_open()) {
        file << id << " " << opp_id " " << newOrder.getId << " " << newOrder.getClientName() << " " << newOrder.getProductName() << " " << newOrder.getStatus() << "\n";
        file.close();
        //TODO: !!Реализовать взаимодействие с SQL
    }
}

// Геттеры для доступа к полям
int Client::getClientId() const {
    return id;
}

// Поиск заказа по статусу
void Client::searchOrderByStatus(const std::string& status) {
    for (const auto& order : orders) {
        if (order.getStatus() == status) { // Предполагается наличие getStatus()
            std::cout << "Заказ со статусом " << status << " найден: " << order.get_order_info() << std::endl;
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

// Возврат заказа
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
void Client::printOrders() {
    for (int i = 0; i < orders.size; i++) {
        std::cout << orders[i].getOrderInfo << std::endl;
    }
}

// Функция для генерации уникального ID клиента
int Client::generateUniqueClientId() {
    static int idCounter = orders.size();
    return ++idCounter;
}
