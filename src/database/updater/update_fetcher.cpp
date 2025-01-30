#include <format>
#include <fstream>
#include <iostream>

#include "update_fetcher.h"

#include "field.h"
#include "query_result.h"

namespace bank::db
{
update_fetcher::update_fetcher(
    const path& source_directory,
    const std::function<void(std::string const&)>& apply,
    const std::function<query_result_ptr(std::string const&)>& retrieve)
    : source_directory_(std::make_unique<path>(source_directory))
    , apply_(apply)
    , retrieve_(retrieve)
{
}

update_result update_fetcher::update() const
{
  const auto available = get_file_list();
  auto applied = receive_applied_files();

  size_t imported_updates = 0;

  for (const auto& query : available) {
    if (auto iter = applied.find(query.filename().string());
        iter != applied.end())
    {
      std::cout << std::format("[INFO]: update {} is already applied",
                               query.filename().string())
                << std::endl;
      applied.erase(iter);
      continue;
    }

    uint32 speed = 0;
    const applied_file_entry applied_file = {query.filename().string(), 0};

    std::cout << std::format("[INFO]: applying update {}...",
                             query.filename().string())
              << std::endl;

    speed = apply(query);
    update_entry(applied_file, speed);

    std::cout << std::format(
        "[INFO]: update {} applied in {}ms", query.filename().string(), speed)
              << std::endl;

    ++imported_updates;
  }

  return update_result(imported_updates);
}

update_fetcher::local_file_storage update_fetcher::get_file_list() const
{
  local_file_storage files;

  fill_file_list(source_directory_->generic_string() + fetch_dir, files);

  return files;
}

update_fetcher::applied_file_storage update_fetcher::receive_applied_files()
    const
{
  applied_file_storage applied_files;

  const auto query_result = retrieve_(
      "SELECT name, unixepoch(applied_at) FROM updates ORDER BY NAME ASC");

  if (!query_result) {
    return applied_files;
  }

  while (query_result->next()) {
    const auto fields = query_result->fetch();
    const applied_file_entry entry = {fields[0].get_string(),
                                      static_cast<uint64>(fields[1].get_int())};

    applied_files.insert(std::make_pair(entry.name, entry));
  }

  delete query_result;  // free memory

  return applied_files;
}

void update_fetcher::fill_file_list(const path& source_directory,
                                    local_file_storage& storage)
{
  for (const auto& file : std::filesystem::directory_iterator(source_directory))
  {
    const auto& entry = file.path();

    if (entry.extension() != ".sql") {
      continue;
    }

    if (std::ranges::find(storage, entry) != storage.end()) {
      throw std::runtime_error("file already exists");
    }

    storage.push_back(entry);
  }
}

uint32 update_fetcher::apply(const path& path) const
{
  auto const begin = std::chrono::high_resolution_clock::now();

  std::ifstream file(path.generic_string());

  if (!file.is_open()) {
    throw std::runtime_error("file could not be opened");
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  const auto query = buffer.str();

  apply_(query);

  auto const end = std::chrono::high_resolution_clock::now();
  auto const elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

  return elapsed.count();
}

void update_fetcher::update_entry(const applied_file_entry& entry,
                                  uint32 speed) const
{
  const auto update =
      std::format(R"(REPLACE INTO updates (name, speed) VALUES("{}", {});)",
                  entry.name,
                  speed);

  apply_(update);
}

}  // namespace bank::db
