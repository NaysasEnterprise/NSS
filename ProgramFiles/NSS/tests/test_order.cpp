#define CATCH_CONFIG_MAIN
#include <../include/catch.hpp>
#include "../include/order.h"
#include "../include/product.h"

TEST_CASE("Order class tests") {
    // Создание тестового продукта
    Product testProduct(101, "Laptop", 1200.99, true);

    // Тестовые данные для заказа
    int orderId = 1;
    std::string clientName = "Alice";
    std::string orderDate = "2024-12-25";
    std::string orderStatus = "Ожидание";
    int quantity = 2;

    // Создание заказа
    Order testOrder(orderId, testProduct, clientName, orderDate, orderStatus, quantity);

    SECTION("Constructor and getters test") {
        REQUIRE(testOrder.getOrderId() == orderId);
        REQUIRE(testOrder.getProductName() == "Laptop");
        REQUIRE(testOrder.getClientName() == clientName);
        REQUIRE(testOrder.getStatus() == orderStatus);
        REQUIRE(testOrder.getOrderQuantity() == quantity);
        REQUIRE(!testOrder.isOrderReady()); // Начальный статус - не готов
    }

    SECTION("Update order status") {
        testOrder.updateOrderStatus("Готов");
        REQUIRE(testOrder.getStatus() == "Готов");
        REQUIRE(testOrder.isOrderReady()); // После изменения статуса на "Готов" заказ готов

        testOrder.updateOrderStatus("Отменён");
        REQUIRE(testOrder.getStatus() == "Отменён");
        REQUIRE(!testOrder.isOrderReady()); // После изменения статуса на другой заказ снова не готов
    }

    SECTION("Get order info") {
        std::string expectedInfo =
            "ID Заказа: 1\n"
            "Продукт: Laptop\n"
            "Клиент: Alice\n"
            "Дата заказа: 2024-12-25\n"
            "Статус: Ожидание\n"
            "Количество: 2\n"
            "Готовность: Нет\n";

        REQUIRE(testOrder.getOrderInfo() == expectedInfo);
    }
}
