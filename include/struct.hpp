#pragma once
#include <string>
#include <utility>
#include <vector>
#include "types/type.hpp"

namespace lexy2 {
struct Struct {
  std::string name;
  std::vector<std::pair<std::string, types::Type>> fields;
};
}  // namespace lexy2
