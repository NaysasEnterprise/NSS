# CMakeList.txt: проект CMake для NSS; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
# NSS executable
add_executable (NSS 
    "NSS/src/NSS.cpp"
    "NSS/socket/socket.cpp"
    "NSS/namespace/functions.cpp"
    "NSS/src/product.cpp"
    "NSS/src/order_pickip_point.cpp"
  )

# Client executable (предположим, что main() определена в clients.cpp)
add_executable (Client 
    "NSS/src/clients.cpp"
  )

add_executable (Server 
    "NSS/server/release.cpp"
  )


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NSS PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
