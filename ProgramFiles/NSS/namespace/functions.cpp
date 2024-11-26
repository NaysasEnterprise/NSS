#include "../include/order_pickup_point.h"
#include <iostream>
#include <random>  // Для генерации случайных чисел
#include <set>     // Для хранения существующих ID
#include <fstream>

using namespace FunctionsOPPControl;

#define CLIENTS_FILE "clients.txt" // Имя файла, где хранятся клиенты

void FunctionsOPPControl::adminMenu() {
    std::cout << "\n--- Администратор ---\n";
    std::cout << "1. Посмотреть все заказы\n";
    std::cout << "2. Найти заказ по ID\n";
    std::cout << "3. Добавить новый заказ\n";
    std::cout << "4. Создать новый ПВЗ\n";
    std::cout << "5. Показать все ПВЗ\n";
    std::cout << "6. Показать всех клиентов\n";
    std::cout << "7. Вернуться в главное меню\n";
    std::cout << "Выберите опцию: ";
}
void FunctionsOPPControl::clientMenu() {
    std::cout << "\n--- Клиент ---\n";
    std::cout << "1. Посмотреть мои заказы\n";
    std::cout << "2. Оформить новый заказ\n";
    std::cout << "3. Найти заказ по ID\n";
    std::cout << "4. Вернуться в главное меню\n";
}

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
int FunctionsOPPControl::generateUniquePVZId() {
    std::set<int> existingIds;
    std::ifstream file("pvz_data.txt");

    // Считываем существующие ID из файла и добавляем их в множество
    if (file.is_open()) {
        int id;
        std::string name;
        double x, y;
        while (file >> id >> name >> x >> y) {
            existingIds.insert(id);
        }
        file.close();
    }

    // Генерация нового уникального ID
    int newId;
    do {
        newId = rand() % 10000 + 1;  // Генерация случайного числа от 1 до 10000
    } while (existingIds.find(newId) != existingIds.end());

    return newId;
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