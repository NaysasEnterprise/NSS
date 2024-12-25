#define CATCH_CONFIG_MAIN
#include <../include/catch.hpp>
#include "../include/clients.h"
#include "../include/order.h"

// Тестирование геттеров и сеттеров
TEST_CASE("Getter and Setter Test for Client", "[client]") {
    Client client(1, "John", "Doe");

    // Проверяем, что значения заданы правильно
    REQUIRE(client.getClientId() == 1);
    REQUIRE(client.getClientFirstName() == "John");
    REQUIRE(client.getClientLastName() == "Doe");
}

// Тестирование добавления заказа
TEST_CASE("Place Order Test", "[client]") {
    Client client(1, "John", "Doe");

    client.placeOrder("Laptop");
    auto orders = client.getClientOrders();

    // Проверяем, что заказ был добавлен
    REQUIRE(orders.size() == 1);
    REQUIRE(orders[0].getProductName() == "Laptop");
}

// Тестирование поиска заказа по ID
TEST_CASE("Search Order By ID Test", "[client]") {
    Client client(1, "John", "Doe");

    client.placeOrder("Laptop");
    client.placeOrder("Smartphone");

    std::vector<Order> orders = client.getClientOrders();
    REQUIRE(orders.size() == 2);

    // Предположим, что Order имеет метод для получения ID
    int orderId = orders[0].getOrderId();
    client.searchOrder(orderId); // Мы не можем проверить вывод в консоль, но можно проверить правильность действий
}

// Тестирование удаления заказа по ID
TEST_CASE("Remove Order Test", "[client]") {
    Client client(1, "John", "Doe");

    client.placeOrder("Laptop");
    auto orders = client.getClientOrders();
    REQUIRE(orders.size() == 1);

    client.removeOrder(orders[0].getOrderId());
    orders = client.getClientOrders();

    // Проверяем, что заказ был удалён
    REQUIRE(orders.size() == 0);
}

// Тестирование возврата заказа
TEST_CASE("Return Order Test", "[client]") {
    Client client(1, "John", "Doe");

    client.placeOrder("Laptop");
    auto orders = client.getClientOrders();
    REQUIRE(orders.size() == 1);

    Order returnedOrder = client.returnOrder("Changed my mind");

    // Проверяем, что заказ был возвращён
    REQUIRE(returnedOrder.getProductName() == "Laptop");
    REQUIRE(client.getClientOrders().size() == 0);
}
