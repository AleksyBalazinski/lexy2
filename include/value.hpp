#pragma once

#include <string>

namespace lexy2 {

struct Value {
  enum class Category { REGISTER, MEMORY, CONSTANT };
  Value(std::string name, int typeID, Category category)
      : name(std::move(name)), typeID(typeID), category(category) {}

  Value(std::string name, int typeID)
      : Value(name, typeID, Category::REGISTER) {}

  std::string name;
  int typeID;
  Category category;
};
}  // namespace lexy2