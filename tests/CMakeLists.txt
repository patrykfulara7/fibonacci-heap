project(tests)

enable_testing()

add_executable(${PROJECT_NAME} main.cxx)

target_link_libraries(${PROJECT_NAME} GTest::gtest_main)

include(GoogleTest)
add_test(${PROJECT_NAME} ${PROJECT_NAME})
# gtest_discover_tests(${PROJECT_NAME})