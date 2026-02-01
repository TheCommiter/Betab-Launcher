#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace vita::data {

struct Notification {
  std::string message;
  std::string item_id;
};

struct RuntimeState {
  int current_page = 0;
  std::vector<std::vector<std::string>> pages;
  std::unordered_map<std::string, std::vector<std::string>> folders;
  std::unordered_map<int, std::string> page_backgrounds;
  std::vector<Notification> notifications;
  std::unordered_map<std::string, double> last_played;
  std::vector<std::string> open_liveareas;

  void EnsureLimits(size_t library_count) const;
  void AddFolder(const std::string &name);
  void AddToFolder(const std::string &folder, const std::string &item_id);
  void RemoveFromFolder(const std::string &folder, const std::string &item_id);
};

class StateStore {
 public:
  explicit StateStore(std::filesystem::path path);
  RuntimeState Load() const;
  void Save(const RuntimeState &state) const;

 private:
  std::filesystem::path path_;
};

}  // namespace vita::data
