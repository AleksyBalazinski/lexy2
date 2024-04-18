#pragma once

#include <string>
#include "types/type.hpp"

namespace lexy2 {

struct Value {
  enum class Category { REGISTER, MEMORY, CONSTANT };
  Value(std::string name, types::Type type, Category category)
      : name(std::move(name)), type(std::move(type)), category(category) {}

  Value(std::string name, types::Type type)
      : Value(name, std::move(type), Category::REGISTER) {}

  std::string name;
  types::Type type;
  Category category;
};
}  // namespace lexy2