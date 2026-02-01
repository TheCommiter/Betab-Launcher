#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace vita::data {

struct LibraryItem {
  std::string item_id;
  std::string title;
  std::string description;
  std::string icon_path;
  std::string hero_path;
  std::string folder;
  std::vector<std::string> cmd_linux;
  std::vector<std::string> cmd_windows;
};

class Library {
 public:
  static Library Load(const std::filesystem::path &path);

  const std::vector<LibraryItem> &items() const { return items_; }

 private:
  std::vector<LibraryItem> items_;
};

}  // namespace vita::data
