#include <string>
#include <vector>
#include "order.h"
#include <iostream>
#pragma once

class PVZ {
private:
    int id;                  // Уникальный ID ПВЗ
    std::string name;        // Название ПВЗ
    double coordX, coordY;   // Координаты ПВЗ

public:
    // Конструктор ПВЗ
    PVZ(int id, const std::string& name, double x, double y);

    // Геттеры
    int getId() const;
    std::string getName() const;
    double getCoordX() const;
    double getCoordY() const;

    // Функция для вывода информации о ПВЗ
    void printPVZInfo() const;
};

namespace FunctionsOPPControl {
    void adminMenu();
    void clientMenu();

    void clientLoginOrRegister();
    int generateUniqueClientId();
    void addClient(const std::string& firstName, const std::string& lastName);
    bool clientExists(const std::string& firstName, const std::string& lastName);

    void showAllClients();
    void showAllPVZ();

    void adminCreatePVZ();
    int generateUniquePVZId();
    void addPVZToFile(const PVZ& pvz);
}