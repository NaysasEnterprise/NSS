#include "../include/clients.h"

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
void Client::printOrders() {
    for (int i = 0; i < orders.size; i++) {
        std::cout << orders[i].getOrderInfo << std::endl;
    }
}

int Client::generateUniqueClientId() {
    static int idCounter = orders.size();
    return ++idCounter;
}
