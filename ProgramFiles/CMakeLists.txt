# CMakeList.txt: проект CMake для NSS; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
# NSS executable

set(Catch2_DIR "../vcpkg_installed/x64-windows/share/Catch2")
find_package(Catch2 CONFIG REQUIRED)


add_executable(test_database "NSS/tests/test_database.cpp"
    "NSS/server/SQL_header.cpp"
    "NSS/socket/socket.cpp"
    "NSS/server/sqlite3.c")

#target_link_libraries(test_order PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(test_database PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

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