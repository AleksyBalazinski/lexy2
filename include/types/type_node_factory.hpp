#pragma once
#include "type_manager.hpp"
#include "types/leaf_node.hpp"

namespace lexy2::types {
class TypeNodeFactory {
  const TypeManager& typeManager;

 public:
  TypeNodeFactory(const TypeManager& typeManager) : typeManager(typeManager) {}

  std::unique_ptr<LeafNode> createLeafNode(TypeManager::TyID typeID) {
    return std::make_unique<LeafNode>(typeID, typeManager);
  }
};
}  // namespace lexy2::types
