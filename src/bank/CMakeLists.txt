add_executable(bank main.cpp)

target_link_libraries(bank PUBLIC database sqlite3)

target_include_directories(bank PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/dep" "${PROJECT_SOURCE_DIR}/src")
