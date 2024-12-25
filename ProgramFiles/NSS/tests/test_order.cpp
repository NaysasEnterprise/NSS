#include <../include/catch.hpp>
#include "../include/product.h"
#include "../include/order.h"﻿
#define CATCH_CONFIG_MAIN


TEST_CASE("Проверка геттеров", "[order]") {
	Product product(0, "Test", 10, true);
	Order order(0, product, "Client", "01.01.1999", "In progress", 0);

	REQUIRE(order.getOrderId() == 0);
	REQUIRE(order.getClientName() == "Client");
	REQUIRE(order.getStatus() == "In progress");
	REQUIRE(order.getOrderQuantity() == 0);
}
TEST_CASE("Проверка на обновление состояния", "[order]") {
	Product product(0, "Test", 10, true);
	Order order(0, product, "Client", "01.01.1999", "In progress", 0);

	order.updateOrderStatus("In progress too");
	REQUIRE(order.isOrderReady());
	order.updateOrderStatus("Готов");
	REQUIRE(order.isOrderReady());
}