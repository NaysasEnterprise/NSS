#define CATCH_CONFIG_MAIN
#include <../include/catch.hpp>
#include "../include/order.h"
#include "../include/order_pickup_point.h"

TEST_CASE("OPP class tests") {
    // Создание ПВЗ
    OPP opp(1, "Central Warehouse", 55.7522, 37.6156);

    SECTION("Constructor and getters") {
        REQUIRE(opp.getId() == 1);
        REQUIRE(opp.getName() == "Central Warehouse");
        REQUIRE(opp.getCoordX() == 55.7522);
        REQUIRE(opp.getCoordY() == 37.6156);
    }

    SECTION("Print OPP info") {
        // Проверяем корректность формата вывода информации о ПВЗ
        std::ostringstream output;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
        opp.printOPPInfo();
        std::cout.rdbuf(oldCoutBuffer);

        std::string expected = "ПВЗ ID: 1, Название: Central Warehouse, Координаты: (55.7522, 37.6156)\n";
        REQUIRE(output.str() == expected);
    }
}

