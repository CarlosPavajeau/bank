cmake_minimum_required(VERSION 3.30)
project(bank)

# build in Release-mode by default if not explicitly set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# set macro-directory
list(APPEND CMAKE_MODULE_PATH
        "${CMAKE_SOURCE_DIR}/cmake/macros")

# build in Release-mode by default if not explicitly set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

include(cmake/options.cmake)

include(GroupSources)
include(AutoCollect)
include(FindSodium)

find_package(Sodium REQUIRED)

if (NOT WITHOUT_GIT)
    find_package(Git 1.7)
endif ()

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

add_subdirectory(dep)
add_subdirectory(src)
add_subdirectory(test)
