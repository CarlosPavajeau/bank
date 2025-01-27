#include <format>

#include "git_revision.h"

#include "revision_data.h"

namespace git_revision
{
char const* get_hash()
{
  return GIT_COMMIT_HASH;
}

char const* get_date()
{
  return GIT_COMMIT_DATE;
}

char const* get_branch()
{
  return GIT_COMMIT_BRANCH;
}

char const* get_build_directory()
{
  return BUILD_CMAKE_BUILD_DIRECTORY;
}

char const* get_source_directory()
{
  return BUILD_CMAKE_SOURCE_DIRECTORY;
}

char const* get_full_version()
{
  return std::format("rev. {}", VER_PRODUCTVERSION).c_str();
}

}  // namespace git_revision
