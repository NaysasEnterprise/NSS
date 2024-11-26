#include "../include/order_pickup_point.h"
#include <string>
#include <vector>
#include "../include/order.h"
#include <iostream>
#include <fstream>
#include <sstream>

// �����������
PVZ::PVZ(int id, const std::string& name, double x, double y)
    : id(id), name(name), coordX(x), coordY(y) {}

// �������
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

// ����� ���������� � ���
void PVZ::printPVZInfo() const {
    std::cout << "��� ID: " << id
        << ", ��������: " << name
        << ", ����������: (" << coordX << ", " << coordY << ")\n";
}