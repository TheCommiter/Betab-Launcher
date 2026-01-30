#include "data/library.h"

#include <fstream>
#include <sstream>

#include "data/json.h"

namespace vita::data {

static std::vector<std::string> ReadStringArray(const JsonValue &value) {
  std::vector<std::string> result;
  for (const auto &entry : value.AsArray()) {
    result.push_back(entry.AsString(""));
  }
  return result;
}

Library Library::Load(const std::filesystem::path &path) {
  Library library;
  std::ifstream file(path);
  if (!file.is_open()) {
    return library;
  }
  std::ostringstream buffer;
  buffer << file.rdbuf();
  JsonParser parser(buffer.str());
  JsonValue root = parser.Parse();
  for (const auto &entry : root.AsArray()) {
    LibraryItem item;
    if (const auto *value = entry.Find("id")) {
      item.item_id = value->AsString("");
    }
    if (const auto *value = entry.Find("title")) {
      item.title = value->AsString("");
    }
    if (const auto *value = entry.Find("desc")) {
      item.description = value->AsString("");
    }
    if (const auto *value = entry.Find("icon")) {
      item.icon_path = value->AsString("");
    }
    if (const auto *value = entry.Find("hero")) {
      item.hero_path = value->AsString("");
    }
    if (const auto *value = entry.Find("folder")) {
      item.folder = value->AsString("");
    }
    if (const auto *value = entry.Find("cmd_linux")) {
      item.cmd_linux = ReadStringArray(*value);
    }
    if (const auto *value = entry.Find("cmd_windows")) {
      item.cmd_windows = ReadStringArray(*value);
    }
    if (!item.item_id.empty()) {
      library.items_.push_back(std::move(item));
    }
  }
  return library;
}

}  // namespace vita::data
