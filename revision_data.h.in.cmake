#ifndef REVISION_DATA_H
#define REVISION_DATA_H

// Git commit information
#define GIT_COMMIT_HASH     "@rev_hash@"
#define GIT_COMMIT_DATE     "@rev_date@"
#define GIT_COMMIT_BRANCH   R"(@rev_branch@)"

// Build OS information
#define BUILD_HOST_SYSTEM                   R"(@TRINITY_BUILD_HOST_SYSTEM@)"
#define BUILD_HOST_SYSTEM_VERSION           R"(@TRINITY_BUILD_HOST_SYSTEM_RELEASE@)"
#cmakedefine BUILD_HOST_DISTRO_NAME              R"(@TRINITY_BUILD_HOST_DISTRO_NAME@)"
#cmakedefine BUILD_HOST_DISTRO_VERSION_ID        R"(@TRINITY_BUILD_HOST_DISTRO_VERSION_ID@)"

// Build target information
#define BUILD_PROCESSOR     R"(@CMAKE_SYSTEM_PROCESSOR@)"

// CMake build information
#define BUILD_CMAKE_COMMAND             R"(@CMAKE_COMMAND@)"
#define BUILD_CMAKE_VERSION             R"(@CMAKE_VERSION@)"
#define BUILD_CMAKE_SOURCE_DIRECTORY    R"(@CMAKE_SOURCE_DIR@)"
#define BUILD_CMAKE_BUILD_DIRECTORY     R"(@BUILDDIR@)"

// Windows resource defines
#define VER_FILEVERSION             0,0,0
#define VER_FILEVERSION_STR         "@rev_hash@ @rev_date@ (@rev_branch@ branch)"
#define VER_PRODUCTVERSION          VER_FILEVERSION
#define VER_PRODUCTVERSION_STR      VER_FILEVERSION_STR

#endif // REVISION_DATA_H