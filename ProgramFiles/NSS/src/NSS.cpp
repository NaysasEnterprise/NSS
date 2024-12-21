// NSS.cpp: ���������� ����� ����� ��� ����������.
#include <iostream>
#include <string>
#include <vector>
#include "../include/order.h"
#include "../include/order_pickup_point.h"
#include "../include/product.h"
#include "../include/clients.h"

using namespace FunctionsOPPControl;

int main() {//
    system("chcp 1251");
    setlocale(LC_ALL, "Russian");  // ��������� ������ ��� ��������� �������� �����

    int choice{};

    while (true) {
        // ������� ����
        startMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: {
            // ���� ��� ��������������
            int adminChoice;
            while (true) {
                FunctionsOPPControl::adminMenu();
                std::cin >> adminChoice;

                switch (adminChoice) {
                case 1:
                    // ����� ������� ��������� ���� �������

                    break;
                case 2:
                    // ����� ������� ������ ������ �� ID
                    std::cout << "������� ID ������ ��� ������: ";
                    int orderId;
                    std::cin >> orderId;
                     // ����� ������ �� ID

                    break;
                case 3:
                    // ����� ������� ���������� ������ ������

                    break;
                case 4:
                    adminCreateOPP();  // �������� ������ ���
                    break;
                case 5:
                    showAllOPP();      // ����� ���� ���
                    break;
                case 6:
                    showAllClients();  // ����� ���� ��������
                    break;
                case 7:
                    std::cout << "������� � ������� ����.\n";
                    goto mainMenu;  // ������� � ������� ����
                default:
                    std::cout << "�������� �����. ����������, ���������� �����.\n";
                }
            }
            break;
        }

        case 2: {
            clientLoginOrRegister(); // ���� ��� ����������� �������

            int clientChoice;
            while (true) {
                clientMenu();
                std::cout << "������� ��� �����: ";
                std::cin >> clientChoice;

                switch (clientChoice) {
                case 1:
                    std::cout << "�������� ��� ������ (����� ����� ������)\n";
                    break;
                case 2:
                    std::cout << "���������� ������ ������ (����� ����� ������)\n";

                    break;
                case 3:
                    std::cout << "������� ID ������ ��� ������: ";
                    // ������ ��� ������ ������ �� ID
                    break;
                case 4:
                    std::cout << "����������� � ������� ����...\n";
                    goto mainMenu;  // ������� � ������� ����
                default:
                    std::cout << "������������ �����. ���������� �����.\n";
                }
            }
            break;
        }

        case 3:
            std::cout << "����� �� ���������...\n";
            return 0;  // ���������� ���������

        default:
            std::cout << "������������ �����. ���������� �����.\n";
        }

    mainMenu:;  // ����� ��� �������� � ������� ����
    }
    return 0;
} 