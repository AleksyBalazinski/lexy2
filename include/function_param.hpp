#pragma once
#include <string>
#include "types/type.hpp"

namespace lexy2 {
struct FunctionParam {
  std::string name;
  types::Type type;

  FunctionParam(std::string name, types::Type type)
      : name(std::move(name)), type(std::move(type)) {}
};
}  // namespace lexy2
