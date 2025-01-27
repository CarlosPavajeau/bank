CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(database ${PRIVATE_SOURCES})

CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

target_include_directories(database
        PUBLIC
        ${CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
        sqlite3
        common
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(database PUBLIC sqlite3)