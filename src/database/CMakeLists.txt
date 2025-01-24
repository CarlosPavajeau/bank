add_library(database db_connection.h db_connection.cpp)

target_link_libraries(database PUBLIC sqlite3)
target_include_directories(database PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/dep")

install(FILES db_connection.h DESTINATION include)
install(TARGETS database LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)