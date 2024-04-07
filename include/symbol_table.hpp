#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "value.hpp"

namespace lexy2 {
class SymbolTable {
  using ScopedTableType = std::unordered_map<std::string, Value>;
  std::vector<ScopedTableType> tableStack;
  std::vector<int> scopeIDBuilder;

  int getCurrentDepth() const;

 public:
  SymbolTable();

  std::pair<ScopedTableType::iterator, int> globalFind(const std::string& id);

  ScopedTableType::iterator currentScopeFind(const std::string& id);

  ScopedTableType::iterator end();

  void insertInCurrentScope(ScopedTableType::value_type&& node);

  void enterNewScope();

  void leaveScope();

  std::string getCurrentScopeID();

  std::string getScopeID(int depth);

  const ScopedTableType& getCurrentScope() const;
};
}  // namespace lexy2