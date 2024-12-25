#include <catch2/catch_test_macros.hpp>
#include "../include/order.h"
#include "../include/product.h"


class MockProduct : public Product {
public:
    MockProduct(const std::string& name) : name(name) {}

    std::string getName() const override {
        return name;
    }

private:
    std::string name;
};

// Тестирование конструктора Order
TEST(OrderTest, ConstructorTest) {
    MockProduct product("Laptop");
    Order order(1, product, "John Doe", "2024-12-21", "Новый", 2);

    EXPECT_EQ(order.getOrderId(), 1);
    EXPECT_EQ(order.getProductName(), "Laptop");
    EXPECT_EQ(order.getClientName(), "John Doe");
    EXPECT_EQ(order.getStatus(), "Новый");
    EXPECT_EQ(order.getOrderQuantity(), 2);
    EXPECT_FALSE(order.isOrderReady());
}

// Тестирование метода updateOrderStatus
TEST(OrderTest, UpdateOrderStatus) {
    MockProduct product("Laptop");
    Order order(1, product, "John Doe", "2024-12-21", "Новый", 2);

    order.updateOrderStatus("Готов");
    EXPECT_EQ(order.getStatus(), "Готов");
    EXPECT_TRUE(order.isOrderReady());

    order.updateOrderStatus("Отправлен");
    EXPECT_EQ(order.getStatus(), "Отправлен");
    EXPECT_FALSE(order.isOrderReady());
}

// Тестирование метода getOrderInfo
TEST(OrderTest, GetOrderInfo) {
    MockProduct product("Laptop");
    Order order(1, product, "John Doe", "2024-12-21", "Новый", 2);

    std::string expectedInfo = "ID Заказа: 1\nПродукт: Laptop\nКлиент: John Doe\nДата заказа: 2024-12-21\nСтатус: Новый\nКоличество: 2\nГотовность: Нет\n";
    EXPECT_EQ(order.getOrderInfo(), expectedInfo);
}

// Тестирование геттеров
TEST(OrderTest, GettersTest) {
    MockProduct product("Phone");
    Order order(2, product, "Alice", "2024-12-22", "Новый", 3);

    EXPECT_EQ(order.getOrderId(), 2);
    EXPECT_EQ(order.getProductName(), "Phone");
    EXPECT_EQ(order.getClientName(), "Alice");
    EXPECT_EQ(order.getStatus(), "Новый");
    EXPECT_EQ(order.getOrderQuantity(), 3);
    EXPECT_FALSE(order.isOrderReady());
}

// Тестирование метода isOrderReady при изменении статуса
TEST(OrderTest, OrderReadyStatus) {
    MockProduct product("Smartwatch");
    Order order(3, product, "Bob", "2024-12-23", "Новый", 1);

    EXPECT_FALSE(order.isOrderReady());

    order.updateOrderStatus("Готов");
    EXPECT_TRUE(order.isOrderReady());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
