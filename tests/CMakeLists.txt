project(tests)

enable_testing()

add_executable(${PROJECT_NAME} main.cxx)

target_link_libraries(${PROJECT_NAME} gtest)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})