#include <string>
#include <vector>
#include "order.h"
#include <iostream>
#pragma once

class OPP {
	//TODO: Продумать работу ПВЗ
};

namespace FunctionsOPPControl {
	void adminMenu();
	void clientMenu();
	bool clientExists(const std::string& firstName, const std::string& lastName);
	void addClient(const std::string& firstName, const std::string& lastName);
	void clientLoginOrRegister();
}