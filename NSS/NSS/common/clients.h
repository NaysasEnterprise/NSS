#include <string>
#include <vector>
#include "order.h"
#include <iostream>

class Client {
private:
    int id;                         // ID клиента
    std::string firstName;           // Имя
    std::string lastName;            // Фамилия
    std::string middleName;          // Отчество
    std::vector<Order> orders;       // Список заказов клиента
    std::vector<Order> receivedOrders; // Список забранных заказов
public:
    // Конструктор класса Client
    Client(int clientId, const std::string& fName, const std::string& lName, const std::string& mName)
        : id(clientId), firstName(fName), lastName(lName), middleName(mName) {}

    // Геттеры для доступа к полям
    int getId() const { return id; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getMiddleName() const { return middleName; }
    std::vector<Order> getOrders() const { return orders; }

    // Сеттеры для изменения полей
    void setFirstName(const std::string& fName) { firstName = fName; }
    void setLastName(const std::string& lName) { lastName = lName; }
    void setMiddleName(const std::string& mName) { middleName = mName; }
    void setId(int clientId) { id = clientId; }

    // Заголовок функции для оформления заказа по названию товара
    void placeOrder(const std::string& productName);

    // Заголовок функции для поиска заказа по параметрам (ID, ФИО, статус заказа)
    void searchOrderByID(int id);
    void searchOrderByFullName(const std::string& fullName); //думаю объединим позже в одну функцию
    void searchOrderByStatus(const std::string& status);

    // Заголовок функции для забора заказа (поиск товара по параметрам)
    void pickOrderByParameters(const std::string& productName, const std::string& clientName);

    void removeOrder(int orderId);
    // Функция для вывода информации о клиенте
    
    Order returnOrder(std::string reason);

    void printClientInfo();
};