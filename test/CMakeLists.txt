enable_testing()

CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bank_test ${PRIVATE_SOURCES})

CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

target_include_directories(bank_test
        PUBLIC
        ${CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
        database
        common
        bank
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(bank_test PUBLIC database common GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(bank_test)
