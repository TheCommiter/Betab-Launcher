#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace vita::data {

class JsonValue {
 public:
  using Array = std::vector<JsonValue>;
  using Object = std::unordered_map<std::string, JsonValue>;

  enum class Type {
    kNull,
    kBool,
    kNumber,
    kString,
    kArray,
    kObject,
  };

  JsonValue();
  explicit JsonValue(bool value);
  explicit JsonValue(double value);
  explicit JsonValue(std::string value);
  explicit JsonValue(Array value);
  explicit JsonValue(Object value);

  Type type() const;
  bool IsNull() const;
  bool AsBool(bool default_value = false) const;
  double AsNumber(double default_value = 0.0) const;
  std::string AsString(const std::string &default_value = "") const;
  const Array &AsArray() const;
  const Object &AsObject() const;

  const JsonValue *Find(const std::string &key) const;

 private:
  using Storage = std::variant<std::monostate, bool, double, std::string, Array, Object>;
  Storage storage_{};
};

class JsonParser {
 public:
  explicit JsonParser(const std::string &input);
  JsonValue Parse();

 private:
  const std::string &input_;
  size_t pos_ = 0;

  void SkipWhitespace();
  bool Match(char expected);
  char Peek() const;
  JsonValue ParseValue();
  JsonValue ParseObject();
  JsonValue ParseArray();
  std::string ParseString();
  double ParseNumber();
  bool ParseLiteral(const std::string &literal);
};

std::string JsonStringify(const JsonValue &value);

}  // namespace vita::data
