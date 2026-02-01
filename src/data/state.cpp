#include "data/state.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "data/json.h"
#include "ui/constants.h"

namespace vita::data {

void RuntimeState::EnsureLimits(size_t library_count) const {
  if (pages.size() > static_cast<size_t>(ui::kMaxPages)) {
    throw std::runtime_error("Too many pages");
  }
  size_t icon_count = 0;
  for (const auto &page : pages) {
    icon_count += page.size();
  }
  for (const auto &entry : folders) {
    icon_count += entry.second.size();
  }
  if (icon_count > static_cast<size_t>(ui::kMaxIcons) || icon_count > library_count) {
    throw std::runtime_error("Too many icons");
  }
}

void RuntimeState::AddFolder(const std::string &name) {
  if (folders.find(name) == folders.end()) {
    folders.emplace(name, std::vector<std::string>{});
  }
}

void RuntimeState::AddToFolder(const std::string &folder, const std::string &item_id) {
  if (item_id.rfind("folder:", 0) == 0) {
    throw std::runtime_error("Folder nesting is not allowed");
  }
  AddFolder(folder);
  auto &items = folders[folder];
  if (std::find(items.begin(), items.end(), item_id) == items.end()) {
    items.push_back(item_id);
  }
}

void RuntimeState::RemoveFromFolder(const std::string &folder, const std::string &item_id) {
  auto iter = folders.find(folder);
  if (iter == folders.end()) {
    return;
  }
  auto &items = iter->second;
  items.erase(std::remove(items.begin(), items.end(), item_id), items.end());
  if (items.empty()) {
    folders.erase(iter);
    for (auto &page : pages) {
      page.erase(
          std::remove(page.begin(), page.end(), std::string("folder:") + folder),
          page.end());
    }
  }
}

static JsonValue ToJson(const RuntimeState &state) {
  JsonValue::Object root;
  root["current_page"] = JsonValue(static_cast<double>(state.current_page));

  JsonValue::Array pages;
  for (const auto &page : state.pages) {
    JsonValue::Array page_array;
    for (const auto &item : page) {
      page_array.emplace_back(item);
    }
    pages.emplace_back(page_array);
  }
  root["pages"] = JsonValue(pages);

  JsonValue::Object folders;
  for (const auto &entry : state.folders) {
    JsonValue::Array items;
    for (const auto &item : entry.second) {
      items.emplace_back(item);
    }
    folders[entry.first] = JsonValue(items);
  }
  root["folders"] = JsonValue(folders);

  JsonValue::Object backgrounds;
  for (const auto &entry : state.page_backgrounds) {
    backgrounds[std::to_string(entry.first)] = JsonValue(entry.second);
  }
  root["backgrounds"] = JsonValue(backgrounds);

  JsonValue::Array notifications;
  for (const auto &note : state.notifications) {
    JsonValue::Object note_obj;
    note_obj["message"] = JsonValue(note.message);
    note_obj["item_id"] = JsonValue(note.item_id);
    notifications.emplace_back(note_obj);
  }
  root["notifications"] = JsonValue(notifications);

  JsonValue::Object last_played;
  for (const auto &entry : state.last_played) {
    last_played[entry.first] = JsonValue(entry.second);
  }
  root["last_played"] = JsonValue(last_played);

  JsonValue::Array open_liveareas;
  for (const auto &item : state.open_liveareas) {
    open_liveareas.emplace_back(item);
  }
  root["open_liveareas"] = JsonValue(open_liveareas);

  return JsonValue(root);
}

static RuntimeState FromJson(const JsonValue &root) {
  RuntimeState state;
  if (const auto *current = root.Find("current_page")) {
    state.current_page = static_cast<int>(current->AsNumber(0));
  }
  if (const auto *pages_value = root.Find("pages")) {
    for (const auto &page_value : pages_value->AsArray()) {
      std::vector<std::string> page;
      for (const auto &entry : page_value.AsArray()) {
        page.push_back(entry.AsString(""));
      }
      state.pages.push_back(std::move(page));
    }
  }
  if (const auto *folders_value = root.Find("folders")) {
    for (const auto &entry : folders_value->AsObject()) {
      std::vector<std::string> items;
      for (const auto &item : entry.second.AsArray()) {
        items.push_back(item.AsString(""));
      }
      state.folders.emplace(entry.first, std::move(items));
    }
  }
  if (const auto *backgrounds_value = root.Find("backgrounds")) {
    for (const auto &entry : backgrounds_value->AsObject()) {
      state.page_backgrounds.emplace(std::stoi(entry.first), entry.second.AsString(""));
    }
  }
  if (const auto *notifications_value = root.Find("notifications")) {
    for (const auto &entry : notifications_value->AsArray()) {
      Notification note;
      if (const auto *message = entry.Find("message")) {
        note.message = message->AsString("");
      }
      if (const auto *item_id = entry.Find("item_id")) {
        note.item_id = item_id->AsString("");
      }
      state.notifications.push_back(std::move(note));
    }
  }
  if (const auto *last_played_value = root.Find("last_played")) {
    for (const auto &entry : last_played_value->AsObject()) {
      state.last_played.emplace(entry.first, entry.second.AsNumber(0.0));
    }
  }
  if (const auto *open_value = root.Find("open_liveareas")) {
    for (const auto &entry : open_value->AsArray()) {
      state.open_liveareas.push_back(entry.AsString(""));
    }
  }
  return state;
}

StateStore::StateStore(std::filesystem::path path) : path_(std::move(path)) {}

RuntimeState StateStore::Load() const {
  std::ifstream file(path_);
  if (!file.is_open()) {
    return RuntimeState{};
  }
  std::ostringstream buffer;
  buffer << file.rdbuf();
  JsonParser parser(buffer.str());
  JsonValue root = parser.Parse();
  return FromJson(root);
}

void StateStore::Save(const RuntimeState &state) const {
  std::ofstream file(path_);
  if (!file.is_open()) {
    return;
  }
  file << JsonStringify(ToJson(state));
}

}  // namespace vita::data
