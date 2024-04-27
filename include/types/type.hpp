#pragma once
#include <memory>
#include <optional>
#include "cloning_visitor.hpp"
#include "type_node.hpp"
#include "type_visitor.hpp"

namespace lexy2::types {
class Type {
  std::unique_ptr<TypeNode> root;

 public:
  Type();

  Type(std::unique_ptr<TypeNode> root);

  Type(const Type& other);

  Type& operator=(const Type& other);

  Type(Type&& other);

  void applyVisitor(TypeVisitor& v) const;

  bool isLeaf() const;

  bool isArray() const;

  std::optional<int> getSimpleTypeId() const;

  std::optional<Type> getPeeledType() const;

  const TypeNode& getRoot() const { return *root; }
};

std::unique_ptr<TypeNode> cloneNode(const TypeNode& node);
}  // namespace lexy2::types
