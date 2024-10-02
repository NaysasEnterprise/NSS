# CMakeList.txt: проект CMake для NSS; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (NSS ".cpp/NSS.cpp" "common/order.h" "common/database_pvz.h" "common/database_clients.h" "common/clients.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NSS PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
