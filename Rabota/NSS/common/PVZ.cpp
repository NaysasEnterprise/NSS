#include "PVZ.h"
#include <string>
#include <vector>
#include "order.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define CLIENTS_FILE "clients.txt" // Имя файла, где хранятся клиенты

// Функция для проверки существования клиента в базе данных
bool FunctionsPVZControl :: clientExists(const std::string& firstName, const std::string& lastName) {
    std::ifstream file(CLIENTS_FILE);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл с клиентами." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string fName, lName;
        if (iss >> fName >> lName) {
            if (fName == firstName && lName == lastName) {
                return true;
            }
        }
    }
    return false;
}

// Функция для добавления нового клиента в базу данных
void FunctionsPVZControl :: addClient(const std::string& firstName, const std::string& lastName) {
    std::ofstream file(CLIENTS_FILE, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи клиента." << std::endl;
        return;
    }

    file << firstName << " " << lastName << "\n";
    file.close();
    std::cout << "Клиент " << firstName << " " << lastName << " успешно добавлен в базу данных." << std::endl;
}
void FunctionsPVZControl :: clientLoginOrRegister() {
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
