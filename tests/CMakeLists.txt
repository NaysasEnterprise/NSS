cmake_minimum_required(VERSION 3.8)
project("tests")

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(test_order "test_order.cpp")
target_link_libraries(test_order PRIVATE Catch2::Catch2WithMain)
add_executable(test_orderPickupPoint "test_orderPickupPoint.cpp")
target_link_libraries(test_orderPickupPoint PRIVATE Catch2::Catch2WithMain)


add_test(NAME ComponentUnitTests COMMAND test_order)