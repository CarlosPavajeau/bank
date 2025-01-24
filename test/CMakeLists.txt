enable_testing()

add_executable(bank_test main.cpp)

include(GoogleTest)
gtest_discover_tests(bank_test)

target_link_libraries(bank_test PUBLIC database GTest::gtest_main)
target_include_directories(bank_test PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/dep" "${PROJECT_SOURCE_DIR}/src")