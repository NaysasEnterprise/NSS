#include <../include/catch.hpp>
#include "../server/sqlite3.h"
#include "../server/SQL_header.h"
#define CATCH_CONFIG_MAIN

sqlite3* db;
const char* dbPath = "test_sql_db.db";
const char* sqlScriptPath = "..\\server\\test_sql_script.sql";

TEST_CASE("Initialization table", "[database]") {
    SECTION("Opening table") {
        bool flag = false;
        sqlite3_open(dbPath, &db);

        SQL_Database::executeSQLScript(db, sqlScriptPath);
        SQL_Database::initializeProductsTable(db);

        const char* checkTableSQL = "SELECT name FROM sqlite_master WHERE type='table' AND name='Users';";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, checkTableSQL, -1, &stmt, nullptr) == SQLITE_OK) {
            flag = sqlite3_step(stmt) == SQLITE_ROW;
            sqlite3_finalize(stmt);
        }

        REQUIRE(sqlite3_prepare_v2(db, checkTableSQL, -1, &stmt, nullptr) == SQLITE_OK);
    }
    SECTION("creating table") {

        // Проверка наличия таблицы
        const char* checkTableSQL = "SELECT name FROM sqlite_master WHERE type='table' AND name='Users';";
        sqlite3_stmt* stmt;
        REQUIRE(sqlite3_prepare_v2(db, checkTableSQL, -1, &stmt, nullptr) == SQLITE_OK);
        sqlite3_finalize(stmt);
    }
}
TEST_CASE("Interaction with server", "[database]") {
    SECTION("Register user")
    {
        SQL_Database::addUser(db, "Ashman Neymarov", "qwerty");
        REQUIRE(SQL_Database::loginUser(db, "Ashman Neymarov", "qwerty"));
    }
    SQL_Database::addPickupPoint(db, "Main office", "Gagarina avenue, 1", 0, 0);
    SQL_Database::addOrder(db, "Ashman Neymarov", 1, { 0,1 });
    SQL_Database::displayPickupPoints(db);
    SQL_Database::displayUsers(db);
    SECTION("Delete user")
    {
        SQL_Database::deleteUser(db, "Ashman Neymarov");
        REQUIRE(!SQL_Database::loginUser(db, "Ashman Neymarov", "qwerty"));
    }
    SQL_Database::deletePickupPoint(db, 1);
}