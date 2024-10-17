#include "clients.h"

// Геттеры
int Client::getId() const { return id; }
std::string Client::getFirstName() const { return firstName; }
std::string Client::getLastName() const { return lastName; }
std::string Client::getMiddleName() const { return middleName; }
std::vector<Order> Client::getOrders() const { return orders; }

// Сеттеры
void Client::setFirstName(const std::string& fName) { firstName = fName; }
void Client::setLastName(const std::string& lName) { lastName = lName; }
void Client::setMiddleName(const std::string& mName) { middleName = mName; }
void Client::setId(int clientId) { id = clientId; }
