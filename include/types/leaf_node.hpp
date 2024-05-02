#pragma once

#include "type_manager.hpp"
#include "type_node.hpp"
#include "type_visitor.hpp"

namespace lexy2::types {
class LeafNode : public TypeNode {
  TypeManager::TyID typeID;

 public:
  LeafNode(TypeManager::TyID typeID, const TypeManager& typeManager);

  virtual void accept(TypeVisitor& v) const override;

  virtual bool isLeaf() const override;

  virtual bool isIntegral() const override;

  virtual bool isFloatingPoint() const override;

  bool isUserDefined() const;

  virtual std::optional<int> getSimpleTypeId() const override;

  virtual std::optional<TypeNode*> getChild() const override;

  TypeManager::TyID getTypeID() const;

  const TypeManager& typeManager;
};
}  // namespace lexy2::types
