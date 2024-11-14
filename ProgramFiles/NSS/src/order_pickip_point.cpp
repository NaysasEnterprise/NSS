#include "../include/order_pickup_point.h"
#include <string>
#include <vector>
#include "../include/order.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define CLIENTS_FILE "clients.txt" // Имя файла, где хранятся клиенты


// Функция для добавления нового клиента в базу данных
void FunctionsOPPControl::clientLoginOrRegister() {
    std::string firstName, lastName;
    std::cout << "Введите имя: ";
    std::cin >> firstName;
    std::cout << "Введите фамилию: ";
    std::cin >> lastName;

    // Проверка, существует ли клиент в базе данных
    if (clientExists(firstName, lastName)) {
        std::cout << "Добро пожаловать обратно, " << firstName << " " << lastName << "!" << std::endl;
    }
    else {
        std::cout << "Клиент не найден. Создаем новый аккаунт." << std::endl;
        addClient(firstName, lastName);
    }
}

int FunctionsOPPControl::generateUniqueClientId() {
    static int lastId = 0;

    // Чтение последнего ID из файла
    std::ifstream file(CLIENTS_FILE);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int id;
            iss >> id;
            if (id > lastId) {
                lastId = id;
            }
        }
    }
    return ++lastId;
}
void FunctionsOPPControl::addClient(const std::string& firstName, const std::string& lastName) {
    int clientId = generateUniqueClientId();
    std::ofstream file(CLIENTS_FILE, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи клиента." << std::endl;
        return;
    }

    // Добавление клиента с пустым списком заказов
    file << clientId << " " << lastName << " " << firstName << " 0 []" << std::endl;
    file.close();

    std::cout << "Клиент " << firstName << " " << lastName << " успешно добавлен с ID " << clientId << "." << std::endl;
}

bool FunctionsOPPControl::clientExists(const std::string& firstName, const std::string& lastName) {
    std::ifstream file(CLIENTS_FILE);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл с клиентами." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int id, numOrders;
        std::string fName, lName;
        iss >> id >> lName >> fName >> numOrders;
        if (fName == firstName && lName == lastName) {
            return true;
        }
    }
    return false;
}
void FunctionsOPPControl::showAllClients() {
    std::ifstream file(CLIENTS_FILE);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл с клиентами." << std::endl;
        return;
    }

    std::string line;
    std::cout << "\n--- Список клиентов ---\n";
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int id, numOrders;
        std::string firstName, lastName;
        std::string orders;

        // Чтение данных из строки
        if (iss >> id >> lastName >> firstName >> numOrders) {
            // Чтение оставшейся части строки как список заказов
            std::getline(iss, orders);
            std::cout << "ID: " << id << "\n"
                << "Фамилия: " << lastName << "\n"
                << "Имя: " << firstName << "\n"
                << "Количество заказов: " << numOrders << "\n"
                << "Заказы: " << orders << "\n\n";
        }
    }
    file.close();
}
