#ifndef GIT_REVISION_H
#define GIT_REVISION_H

namespace git_revision
{
char const* get_hash();
char const* get_date();
char const* get_branch();
char const* get_build_directory();
char const* get_source_directory();
char const* get_full_version();
}  // namespace git_revision

#endif  // GIT_REVISION_H
