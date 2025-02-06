#ifndef UPDATE_FETCHER_H
#define UPDATE_FETCHER_H
#include <filesystem>
#include <functional>

#include "database_env.h"
#include "define.h"

namespace bank::db
{

struct update_result
{
  explicit update_result(const size_t updated_)
      : updated(updated_)
  {
  }

  update_result() = default;

  size_t updated = 0;
};

class update_fetcher
{
  typedef std::filesystem::path path;

public:
  update_fetcher(
      const path& source_directory,
      const std::function<void(std::string const&)>& apply,
      const std::function<query_result(std::string const&)>& retrieve);
  ~update_fetcher() = default;

  [[nodiscard]] update_result update() const;

private:
  struct applied_file_entry
  {
    const std::string name;
    const uint64 applied_at;
  };

  typedef std::vector<path> local_file_storage;
  typedef std::unordered_map<std::string, applied_file_entry>
      applied_file_storage;

  [[nodiscard]] local_file_storage get_file_list() const;
  [[nodiscard]] applied_file_storage receive_applied_files() const;

  static void fill_file_list(const path& source_directory,
                             local_file_storage& storage);

  [[nodiscard]] uint32 apply(const path& path) const;

  void update_entry(const applied_file_entry& entry, uint32 speed = 0) const;

  std::unique_ptr<path> source_directory_;

  const std::function<void(std::string const&)> apply_;
  const std::function<query_result(std::string const&)> retrieve_;

  static constexpr auto fetch_dir = "/sql/updates";
};
}  // namespace bank::db

#endif  // UPDATE_FETCHER_H
