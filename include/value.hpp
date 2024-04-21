#pragma once

#include <string>
#include "types/type.hpp"

namespace lexy2 {

struct Value {
  enum class Category { REGISTER, MEMORY, CONSTANT, INTERNAL_PTR };

  Value(std::string name, types::Type type, Category category,
        std::string bareName)
      : name(name),
        type(std::move(type)),
        category(category),
        bareName(std::move(bareName)) {}

  Value(std::string name, types::Type type, Category category)
      : Value(name, std::move(type), category, name) {}

  Value(std::string name, types::Type type)
      : Value(name, std::move(type), Category::REGISTER, name) {}

  std::string name;
  std::string bareName;
  types::Type type;
  Category category;
};
}  // namespace lexy2