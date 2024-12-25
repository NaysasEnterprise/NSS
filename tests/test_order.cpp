#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "order.h"
#include "product.h"
#include<cstdlib>

TEST_CASE("Проверка геттеров", "[order]") {
	Product product = new Product(0, "Test", 10d, true);
	Order order = new Order(0, product, "Client", "01.01.1999", "In progress", 0);

	cout << order.getOrderId() << endl;
	REQUIRE(order.getOrderId() == 0);
	REQUIRE(order.getOrderName() == "Test");
	REQUIRE(order.getClientName() == "Client");
	REQUIRE(order.getStatus() == "In progress");
	REQUIRE(order.getOrderQuantity() == 0)
}
TEST_CASE("Проверка на обновление состояния", "[order]") {
	Product product = new Product(0, "Test", 10d, true);
	Order order = new Order(0, product, "Client", "01.01.1999", "In progress", 0);

	order.updateOrderStatus("In progress too");
	REQUIRE(order.isOrderReady());
	order.updateOrderStatus("Готов");
	REQUIRE(order.isOrderReady());
}