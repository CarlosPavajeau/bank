add_custom_target(revision_data.h ALL
        COMMAND "${CMAKE_COMMAND}" -DBUILDDIR="${CMAKE_BINARY_DIR}" -P "${CMAKE_SOURCE_DIR}/cmake/genrev.cmake" "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

set_target_properties(revision_data.h
        PROPERTIES
        FOLDER
        "bank")