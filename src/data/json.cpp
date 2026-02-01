#include "data/json.h"

#include <cctype>
#include <sstream>
#include <stdexcept>

namespace vita::data {

JsonValue::JsonValue() = default;
JsonValue::JsonValue(bool value) : storage_(value) {}
JsonValue::JsonValue(double value) : storage_(value) {}
JsonValue::JsonValue(std::string value) : storage_(std::move(value)) {}
JsonValue::JsonValue(Array value) : storage_(std::move(value)) {}
JsonValue::JsonValue(Object value) : storage_(std::move(value)) {}

JsonValue::Type JsonValue::type() const {
  switch (storage_.index()) {
    case 0:
      return Type::kNull;
    case 1:
      return Type::kBool;
    case 2:
      return Type::kNumber;
    case 3:
      return Type::kString;
    case 4:
      return Type::kArray;
    case 5:
      return Type::kObject;
    default:
      return Type::kNull;
  }
}

bool JsonValue::IsNull() const {
  return std::holds_alternative<std::monostate>(storage_);
}

bool JsonValue::AsBool(bool default_value) const {
  if (auto value = std::get_if<bool>(&storage_)) {
    return *value;
  }
  return default_value;
}

double JsonValue::AsNumber(double default_value) const {
  if (auto value = std::get_if<double>(&storage_)) {
    return *value;
  }
  return default_value;
}

std::string JsonValue::AsString(const std::string &default_value) const {
  if (auto value = std::get_if<std::string>(&storage_)) {
    return *value;
  }
  return default_value;
}

const JsonValue::Array &JsonValue::AsArray() const {
  static const Array kEmpty;
  if (auto value = std::get_if<Array>(&storage_)) {
    return *value;
  }
  return kEmpty;
}

const JsonValue::Object &JsonValue::AsObject() const {
  static const Object kEmpty;
  if (auto value = std::get_if<Object>(&storage_)) {
    return *value;
  }
  return kEmpty;
}

const JsonValue *JsonValue::Find(const std::string &key) const {
  if (auto object = std::get_if<Object>(&storage_)) {
    auto iter = object->find(key);
    if (iter != object->end()) {
      return &iter->second;
    }
  }
  return nullptr;
}

JsonParser::JsonParser(const std::string &input) : input_(input) {}

JsonValue JsonParser::Parse() {
  SkipWhitespace();
  JsonValue value = ParseValue();
  SkipWhitespace();
  return value;
}

void JsonParser::SkipWhitespace() {
  while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) {
    ++pos_;
  }
}

bool JsonParser::Match(char expected) {
  if (pos_ < input_.size() && input_[pos_] == expected) {
    ++pos_;
    return true;
  }
  return false;
}

char JsonParser::Peek() const {
  if (pos_ < input_.size()) {
    return input_[pos_];
  }
  return '\0';
}

JsonValue JsonParser::ParseValue() {
  SkipWhitespace();
  char ch = Peek();
  if (ch == '{') {
    return ParseObject();
  }
  if (ch == '[') {
    return ParseArray();
  }
  if (ch == '"') {
    return JsonValue(ParseString());
  }
  if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '-') {
    return JsonValue(ParseNumber());
  }
  if (ParseLiteral("true")) {
    return JsonValue(true);
  }
  if (ParseLiteral("false")) {
    return JsonValue(false);
  }
  if (ParseLiteral("null")) {
    return JsonValue();
  }
  throw std::runtime_error("Unexpected JSON token");
}

JsonValue JsonParser::ParseObject() {
  if (!Match('{')) {
    throw std::runtime_error("Expected '{'");
  }
  JsonValue::Object result;
  SkipWhitespace();
  if (Match('}')) {
    return JsonValue(result);
  }
  while (true) {
    SkipWhitespace();
    if (Peek() != '"') {
      throw std::runtime_error("Expected string key");
    }
    std::string key = ParseString();
    SkipWhitespace();
    if (!Match(':')) {
      throw std::runtime_error("Expected ':' after key");
    }
    SkipWhitespace();
    JsonValue value = ParseValue();
    result.emplace(std::move(key), std::move(value));
    SkipWhitespace();
    if (Match('}')) {
      break;
    }
    if (!Match(',')) {
      throw std::runtime_error("Expected ',' between object entries");
    }
  }
  return JsonValue(result);
}

JsonValue JsonParser::ParseArray() {
  if (!Match('[')) {
    throw std::runtime_error("Expected '['");
  }
  JsonValue::Array result;
  SkipWhitespace();
  if (Match(']')) {
    return JsonValue(result);
  }
  while (true) {
    SkipWhitespace();
    result.push_back(ParseValue());
    SkipWhitespace();
    if (Match(']')) {
      break;
    }
    if (!Match(',')) {
      throw std::runtime_error("Expected ',' between array entries");
    }
  }
  return JsonValue(result);
}

std::string JsonParser::ParseString() {
  if (!Match('"')) {
    throw std::runtime_error("Expected string opening quote");
  }
  std::string result;
  while (pos_ < input_.size()) {
    char ch = input_[pos_++];
    if (ch == '"') {
      break;
    }
    if (ch == '\\') {
      if (pos_ >= input_.size()) {
        break;
      }
      char escaped = input_[pos_++];
      switch (escaped) {
        case '"':
        case '\\':
        case '/':
          result.push_back(escaped);
          break;
        case 'b':
          result.push_back('\b');
          break;
        case 'f':
          result.push_back('\f');
          break;
        case 'n':
          result.push_back('\n');
          break;
        case 'r':
          result.push_back('\r');
          break;
        case 't':
          result.push_back('\t');
          break;
        default:
          result.push_back(escaped);
          break;
      }
      continue;
    }
    result.push_back(ch);
  }
  return result;
}

double JsonParser::ParseNumber() {
  size_t start = pos_;
  if (Peek() == '-') {
    ++pos_;
  }
  while (pos_ < input_.size() && std::isdigit(static_cast<unsigned char>(input_[pos_]))) {
    ++pos_;
  }
  if (Peek() == '.') {
    ++pos_;
    while (pos_ < input_.size() && std::isdigit(static_cast<unsigned char>(input_[pos_]))) {
      ++pos_;
    }
  }
  std::string token = input_.substr(start, pos_ - start);
  return std::stod(token);
}

bool JsonParser::ParseLiteral(const std::string &literal) {
  if (input_.compare(pos_, literal.size(), literal) == 0) {
    pos_ += literal.size();
    return true;
  }
  return false;
}

static void JsonStringifyValue(const JsonValue &value, std::ostringstream &out) {
  switch (value.type()) {
    case JsonValue::Type::kNull:
      out << "null";
      return;
    case JsonValue::Type::kBool:
      out << (value.AsBool() ? "true" : "false");
      return;
    case JsonValue::Type::kNumber:
      out << value.AsNumber();
      return;
    case JsonValue::Type::kString: {
      out << '"';
      for (char ch : value.AsString()) {
        if (ch == '"' || ch == '\\') {
          out << '\\';
        }
        out << ch;
      }
      out << '"';
      return;
    }
    case JsonValue::Type::kArray: {
      out << '[';
      const auto &array = value.AsArray();
      for (size_t i = 0; i < array.size(); ++i) {
        JsonStringifyValue(array[i], out);
        if (i + 1 < array.size()) {
          out << ',';
        }
      }
      out << ']';
      return;
    }
    case JsonValue::Type::kObject: {
      out << '{';
      const auto &obj = value.AsObject();
      size_t index = 0;
      for (const auto &entry : obj) {
        out << '"' << entry.first << '"' << ':';
        JsonStringifyValue(entry.second, out);
        if (++index < obj.size()) {
          out << ',';
        }
      }
      out << '}';
      return;
    }
  }
}

std::string JsonStringify(const JsonValue &value) {
  std::ostringstream out;
  JsonStringifyValue(value, out);
  return out.str();
}

}  // namespace vita::data
