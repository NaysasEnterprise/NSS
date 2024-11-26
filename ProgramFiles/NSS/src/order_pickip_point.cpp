#include "../include/order_pickup_point.h"
#include <string>
#include <vector>
#include "../include/order.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Конструктор
PVZ::PVZ(int id, const std::string& name, double x, double y)
    : id(id), name(name), coordX(x), coordY(y) {}

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