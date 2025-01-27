CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bank ${PRIVATE_SOURCES})

CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

target_include_directories(bank
        PUBLIC
        ${CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
        database
        common
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(bank PUBLIC database common)
