project(tests)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(${PROJECT_NAME} 
    main.cxx
)

target_link_libraries(${PROJECT_NAME}
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(${PROJECT_NAME})