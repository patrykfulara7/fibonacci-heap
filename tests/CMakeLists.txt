enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

add_executable(tests main.cxx)

target_link_libraries(tests gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)