#pragma once
#include <optional>
#include "type_visitor.hpp"

namespace lexy2::types {
class TypeNode {
 public:
  virtual void accept(TypeVisitor& v) const = 0;
  virtual bool isLeaf() const = 0;
  virtual std::optional<int> getSimpleTypeId() const = 0;
  virtual std::optional<TypeNode*> getChild() const = 0;
};
}  // namespace lexy2::types
