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
    "NSS/src/clients.cpp" "NSS/server/SQL_header.cpp")

    add_executable (NSS_Server 
    "NSS/src/NSS_server.cpp"
    "NSS/socket/socket.cpp"
    "NSS/namespace/functionsOppControl.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
    "NSS/src/order.cpp"
    "NSS/src/clients.cpp"
   "NSS/server/sqlite3.h" "NSS/server/sqlite3.c" "NSS/src/client_server.cpp" "NSS/server/SQL_header.cpp")

# Client executable (предположим, что main() определена в clients.cpp)
add_executable (Client 
    "NSS/src/client_server.cpp" "NSS/server/SQL_header.cpp")

add_executable (Server 
    "NSS/server/release.cpp"
   "NSS/server/sqlite3.h" "NSS/server/sqlite3.c" "NSS/src/client_server.cpp" "NSS/server/SQL_header.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NSS PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
