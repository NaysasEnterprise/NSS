# CMakeList.txt: проект CMake для NSS; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
# NSS executable

add_executable (NSS
    "NSS/src/NSS.cpp"
    "NSS/namespace/functionsOppControl.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
    "NSS/src/order.cpp"
    "NSS/src/clients.cpp"
    "NSS/tests/test_main.cpp")

add_executable (NSS_Server 
    "NSS/src/NSS_server.cpp"
    "NSS/socket/socket.cpp"
    "NSS/namespace/functionsOppControl.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
    "NSS/src/order.cpp"
    "NSS/src/clients.cpp"
    "NSS/server/sqlite3.h" 
    "NSS/server/sqlite3.c"  
    "NSS/server/SQL_header.cpp")

# Линкуем с библиотеками Google Test и Google Mock
target_link_libraries(test_ gtest gtest_main)

enable_testing()

# Создайте цель для тестов
add_executable(test_clients tests/test_clients.cpp)
//add_executable(test_order_pickup_point tests/test_order_pickup_point.cpp)
add_executable(test_product tests/test_product.cpp)

# Линкуйте Google Test
target_link_libraries(test_clients GTest::GTest GTest::Main)
//target_link_libraries(test_order_pickup_point GTest::GTest GTest::Main)
target_link_libraries(test_product GTest::GTest GTest::Main)

# Добавьте тесты в CTest
add_test(NAME TestClients COMMAND test_clients)
//add_test(NAME TestOrderPickupPoint COMMAND test_order_pickup_point)
add_test(NAME TestProduct COMMAND test_product)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NSS_Server PROPERTY CXX_STANDARD 20)
endif()