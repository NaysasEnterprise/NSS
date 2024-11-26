#include "../include/order_pickup_point.h"
#include <string>
#include <vector>
#include "../include/order.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define CLIENTS_FILE "clients.txt"                    


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


// Конструктор
PVZ::PVZ(int id, const std::string& name, double x, double y)
    : id(id), name(name), coordX(x), coordY(y) {
}

// Геттеры
int PVZ::getId() const {
    return id;
}
std::string PVZ::getName() const {
    return name;
}
double PVZ::getCoordX() const {
    return coordX;
}
double PVZ::getCoordY() const {
    return coordY;
}

// Вывод информации о ПВЗ
void PVZ::printPVZInfo() const {
    std::cout << "ПВЗ ID: " << id
        << ", Название: " << name
        << ", Координаты: (" << coordX << ", " << coordY << ")\n";
}

void FunctionsOPPControl::addPVZToFile(const PVZ& pvz) {
    std::ofstream file("pvz_data.txt", std::ios::app);
    if (file.is_open()) {
        file << pvz.getId() << " " << pvz.getName() << " "
            << pvz.getCoordX() << " " << pvz.getCoordY() << "\n";
        file.close();
    }
    else {
        std::cerr << "Ошибка: Не удалось открыть файл для записи.\n";
    }
}

// Создание нового ПВЗ администратором
void FunctionsOPPControl::adminCreatePVZ() {
    int id = generateUniquePVZId();  // Автоматическое присвоение ID
    double x, y;
    std::string name;

    std::cout << "Создание нового ПВЗ\n";
    std::cout << "ID ПВЗ: " << id << " (автоматически присвоен)\n";
    std::cout << "Введите название ПВЗ: ";
    std::cin >> name;
    std::cout << "Введите координату X: ";
    std::cin >> x;
    std::cout << "Введите координату Y: ";
    std::cin >> y;

    PVZ newPVZ(id, name, x, y);
    addPVZToFile(newPVZ);
    std::cout << "ПВЗ успешно создан и добавлен в файл.\n";
}


// Чтение всех ПВЗ из файла и вывод на экран
void FunctionsOPPControl::showAllPVZ() {
    std::ifstream file("pvz_data.txt");
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл.\n";
        return;
    }

    std::cout << "\nСписок всех ПВЗ:\n";
    int id, x, y;
    std::string name;
    while (file >> id >> name >> x >> y) {
        std::cout << "ID: " << id << ", Название: " << name
            << ", Координаты: (" << x << ", " << y << ")\n";
    }
    file.close();
}