#include <gtest/gtest.h>
#include "../include/product.h"

// Тест конструктора и геттеров
TEST(ProductTests, ConstructorAndGetters) {
    Product product(1, "TestProduct", 99.99, true);

    EXPECT_EQ(product.getId(), 1);
    EXPECT_EQ(product.getName(), "TestProduct");
    EXPECT_DOUBLE_EQ(product.getPrice(), 99.99);
    EXPECT_TRUE(product.getAvailability());
}

// Тест сеттеров
TEST(ProductTests, Setters) {
    Product product(1, "TestProduct", 99.99, true);

    product.setId(2);
    product.setName("UpdatedProduct");
    product.setPrice(199.99);
    product.setAvailability(false);

    EXPECT_EQ(product.getId(), 2);
    EXPECT_EQ(product.getName(), "UpdatedProduct");
    EXPECT_DOUBLE_EQ(product.getPrice(), 199.99);
    EXPECT_FALSE(product.getAvailability());
}

// Тест перегрузки оператора вывода
TEST(ProductTests, OutputOperator) {
    Product product(1, "TestProduct", 99.99, true);
    std::ostringstream oss;
    oss << product;

    std::string expectedOutput =
        "ID товара: 1\n"
        "Название: TestProduct\n"
        "Цена: 99.99\n"
        "Наличие: Да\n";

    EXPECT_EQ(oss.str(), expectedOutput);
}

// Тест генерации продуктов
TEST(ProductTests, GenerateProducts) {
    int productCount = 5;
    auto products = Product::generateProducts(productCount);

    EXPECT_EQ(products.size(), productCount);

    for (int i = 0; i < productCount; ++i) {
        EXPECT_EQ(products[i].getId(), i + 1);
        EXPECT_TRUE(!products[i].getName().empty());
        EXPECT_GT(products[i].getPrice(), 10.0);  // Цена больше минимальной
        EXPECT_LT(products[i].getPrice(), 500.0); // Цена меньше максимальной
    }
}
