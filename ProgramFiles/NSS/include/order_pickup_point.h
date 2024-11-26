#include <string>
#include <vector>
#include "order.h"
#include <iostream>
#pragma once

class OPP {
private:
    struct Client {
        std::string firstName;
        std::string lastName;
        std::vector<Order> orders;
    };

    std::vector<Client> clients; // Список клиентов
    std::vector<Order> inventory; // Заказы, находящиеся на ПВЗ

public:
    // Добавление клиента
    void addClient(const std::string& firstName, const std::string& lastName) {
        if (!clientExists(firstName, lastName)) {
            clients.push_back({ firstName, lastName, {} });
            std::cout << "Клиент " << firstName << " " << lastName << " успешно добавлен.\n";
        }
        else {
            std::cout << "Клиент уже существует.\n";
        }
    }

    // Проверка наличия клиента
    bool clientExists(const std::string& firstName, const std::string& lastName) const {
        for (const auto& client : clients) {
            if (client.firstName == firstName && client.lastName == lastName) {
                return true;
            }
        }
        return false;
    }

    // Добавление заказа в инвентарь ПВЗ
    void addOrderToInventory(const Order& order) {
        inventory.push_back(order);
        std::cout << "Заказ с ID " << order.getOrderID() << " добавлен в инвентарь ПВЗ.\n";
    }

    // Получение всех заказов клиента
    std::vector<Order> getClientOrders(const std::string& firstName, const std::string& lastName) const {
        for (const auto& client : clients) {
            if (client.firstName == firstName && client.lastName == lastName) {
                return client.orders;
            }
        }
        std::cout << "Клиент не найден.\n";
        return {};
    }

    // Удаление заказа после получения
    bool removeOrder(const std::string& orderID) {
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            if (it->getOrderID() == orderID) {
                inventory.erase(it);
                std::cout << "Заказ с ID " << orderID << " удалён из инвентаря.\n";
                return true;
            }
        }
        std::cout << "Заказ с ID " << orderID << " не найден в инвентаре.\n";
        return false;
    }

    // Вывод информации о всех клиентах
    void printClients() const {
        std::cout << "Список клиентов:\n";
        for (const auto& client : clients) {
            std::cout << "- " << client.firstName << " " << client.lastName << "\n";
        }
    }

    // Вывод информации о всех заказах в ПВЗ
    void printInventory() const {
        std::cout << "Список заказов в ПВЗ:\n";
        for (const auto& order : inventory) {
            std::cout << "- Заказ ID: " << order.getOrderID() << "\n";
        }
    }
};
};

namespace FunctionsOPPControl {
	void adminMenu();
	void clientMenu();
	bool clientExists(const std::string& firstName, const std::string& lastName);
	void addClient(const std::string& firstName, const std::string& lastName);
	void clientLoginOrRegister();
}