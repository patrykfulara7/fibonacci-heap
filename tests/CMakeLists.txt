add_executable(tests main.cxx)

target_link_libraries(tests gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
