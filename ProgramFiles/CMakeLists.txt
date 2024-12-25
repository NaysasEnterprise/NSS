# CMakeList.txt: проект CMake для NSS; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
# NSS executable

set(Catch2_DIR "../vcpkg_installed/x64-windows/share/Catch2")
find_package(Catch2 CONFIG REQUIRED)

# тест для client.cpp
add_executable(test_client "NSS/tests/test_client.cpp"
    "NSS/namespace/functionsOppControl.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
    "NSS/src/order.cpp"
    "NSS/src/clients.cpp")

# тест для order.cpp
add_executable(test_order "NSS/tests/test_order.cpp"
    "NSS/namespace/functionsOppControl.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
    "NSS/src/order.cpp"
    "NSS/src/clients.cpp")

# тест для product.cpp
add_executable(test_product "NSS/tests/test_product.cpp"
    "NSS/namespace/functionsOppControl.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
    "NSS/src/order.cpp"
    "NSS/src/clients.cpp")

# тест для order_pickup_point.cpp
add_executable(test_OPP "NSS/tests/test_OPP.cpp"
    "NSS/namespace/functionsOppControl.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
    "NSS/src/order.cpp"
    "NSS/src/clients.cpp")

target_link_libraries(test_client PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(test_order PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(test_product PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(test_OPP PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

add_executable (NSS
    "NSS/src/NSS.cpp"
    "NSS/namespace/functionsOppControl.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
    "NSS/src/order.cpp"
    "NSS/src/clients.cpp"
)
add_executable (NSS_Server 
"NSS/src/NSS_server.cpp"
"NSS/socket/socket.cpp"
"NSS/namespace/functionsOppControl.cpp"
"NSS/src/product.cpp"
"NSS/src/order_pickip_point.cpp"
"NSS/src/order.cpp"
"NSS/src/clients.cpp"
"NSS/server/sqlite3.h" "NSS/server/sqlite3.c"  "NSS/server/SQL_header.cpp"
)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NSS_Server PROPERTY CXX_STANDARD 20)
endif()