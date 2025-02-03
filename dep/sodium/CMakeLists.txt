if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif ()

option(SODIUM_DISABLE_TESTS "Disable tests" OFF)
option(SODIUM_MINIMAL "Only compile the minimum set of functions required for the high-level API" OFF)
option(SODIUM_ENABLE_BLOCKING_RANDOM "Enable this switch only if /dev/urandom is totally broken on the target platform" OFF)

CollectSourceFiles("${CMAKE_CURRENT_SOURCE_DIR}/libsodium" PRIVATE_SOURCES)
GroupSources("${CMAKE_CURRENT_SOURCE_DIR}/libsodium")

add_library(sodium ${PRIVATE_SOURCES})

CollectIncludeDirectories("${CMAKE_CURRENT_SOURCE_DIR}/libsodium/include" PUBLIC_INCLUDES)
CollectIncludeDirectories("${CMAKE_CURRENT_SOURCE_DIR}/libsodium" PRIVATE_INCLUDES)

target_include_directories(sodium
        PUBLIC
        ${CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PRIVATE_INCLUDES}
)

target_compile_definitions(sodium
        PUBLIC
        $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:SODIUM_STATIC>
        $<$<BOOL:${SODIUM_MINIMAL}>:SODIUM_LIBRARY_MINIMAL>
        PRIVATE
        CONFIGURED
        $<$<BOOL:${BUILD_SHARED_LIBS}>:SODIUM_DLL_EXPORT>
        $<$<BOOL:${SODIUM_ENABLE_BLOCKING_RANDOM}>:USE_BLOCKING_RANDOM>
        $<$<BOOL:${SODIUM_MINIMAL}>:MINIMAL>
        $<$<C_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

set(VERSION "1.20.0")
set(SODIUM_LIBRARY_VERSION_MAJOR 1)
set(SODIUM_LIBRARY_VERSION_MINOR 20)
if (SODIUM_MINIMAL)
    set(SODIUM_LIBRARY_MINIMAL_DEF "#define SODIUM_LIBRARY_MINIMAL 1")
endif ()

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/libsodium/include/sodium/version.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/libsodium/include/sodium/version.h"
        @ONLY
)

