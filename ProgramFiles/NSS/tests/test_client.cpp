#include <gtest/gtest.h>
#include "../include/clients.h"
#include "../include/order.h"

// Тестирование геттеров и сеттеров
TEST(ClientTest, GetterSetterTest) {
    Client client;

    client.setClientId(1);
    client.setClientFirstName("John");
    client.setClientLastName("Doe");

    // Проверяем, что значения заданы правильно
    EXPECT_EQ(client.getClientId(), 1);
    EXPECT_EQ(client.getClientFirstName(), "John");
    EXPECT_EQ(client.getClientLastName(), "Doe");
}

// Тестирование добавления заказа
TEST(ClientTest, PlaceOrderTest) {
    Client client;
    client.setClientId(1);
    client.setClientFirstName("John");
    client.setClientLastName("Doe");

    client.placeOrder("Laptop");
    auto orders = client.getClientOrders();

    // Проверяем, что заказ был добавлен
    ASSERT_EQ(orders.size(), 1);
    EXPECT_EQ(orders[0].getProductName(), "Laptop");
}

// Тестирование поиска заказа по ID
TEST(ClientTest, SearchOrderByIdTest) {
    Client client;
    client.setClientId(1);
    client.setClientFirstName("John");
    client.setClientLastName("Doe");

    client.placeOrder("Laptop");
    client.placeOrder("Smartphone");

    std::vector<Order> orders = client.getClientOrders();
    EXPECT_EQ(orders.size(), 2);

    // Предположим, что Order имеет метод для получения ID
    int orderId = orders[0].getOrderId();
    client.searchOrder(orderId); // Мы не можем проверить вывод в консоль, но можно проверить правильность действий
}

// Тестирование удаления заказа по ID
TEST(ClientTest, RemoveOrderTest) {
    Client client;
    client.setClientId(1);
    client.setClientFirstName("John");
    client.setClientLastName("Doe");

    client.placeOrder("Laptop");
    auto orders = client.getClientOrders();
    ASSERT_EQ(orders.size(), 1);

    client.removeOrder(orders[0].getOrderId());
    orders = client.getClientOrders();

    // Проверяем, что заказ был удалён
    EXPECT_EQ(orders.size(), 0);
}

// Тестирование возврата заказа
TEST(ClientTest, ReturnOrderTest) {
    Client client;
    client.setClientId(1);
    client.setClientFirstName("John");
    client.setClientLastName("Doe");

    client.placeOrder("Laptop");
    auto orders = client.getClientOrders();
    ASSERT_EQ(orders.size(), 1);

    Order returnedOrder = client.returnOrder("Changed my mind");

    // Проверяем, что заказ был возвращён
    EXPECT_EQ(returnedOrder.getProductName(), "Laptop");
    EXPECT_EQ(client.getClientOrders().size(), 0);
}
