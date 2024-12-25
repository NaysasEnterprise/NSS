#define CATCH_CONFIG_MAIN
#include <../include/catch.hpp>
#include "../include/product.h"

bool almostEqual(double a, double b, double epsilon = 0.01) {
    return std::abs(a - b) < epsilon;
}

TEST_CASE("Product Constructor initializes correctly", "[Product]") {
    Product product(1, "Laptop", 999.99, true);

    REQUIRE(product.getId() == 1);
    REQUIRE(product.getName() == "Laptop");
    REQUIRE(almostEqual(product.getPrice(), 999.99));
    REQUIRE(product.getAvailability() == true);
}

TEST_CASE("Product Setters and Getters work correctly", "[Product]") {
    Product product(0, "", 0.0, false);

    product.setId(2);
    product.setName("Smartphone");
    product.setPrice(499.99);
    product.setAvailability(true);

    REQUIRE(product.getId() == 2);
    REQUIRE(product.getName() == "Smartphone");
    REQUIRE(almostEqual(product.getPrice(), 999.99));
    REQUIRE(product.getAvailability() == true);
}

TEST_CASE("Operator<< outputs correctly", "[Product]") {
    Product product(3, "Tablet", 299.99, true);
    std::ostringstream oss;
    oss << product;

    std::string expectedOutput = "Product ID: 3, Name: Tablet, Price: 299.99, Available: Yes";
    REQUIRE(oss.str() == expectedOutput);
}

TEST_CASE("Product::generateProducts generates correct number of products", "[Product]") {
    int count = 5;
    auto products = Product::generateProducts(count);

    REQUIRE(products.size() == count);

    for (int i = 0; i < count; ++i) {
        REQUIRE(products[i].getId() == i + 1);
        REQUIRE(!products[i].getName().empty());
        REQUIRE(products[i].getPrice() > 0);
    }
}
