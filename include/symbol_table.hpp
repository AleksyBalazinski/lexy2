#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "value.hpp"

namespace lexy2 {
class SymbolTable {
  using ScopedTableType = std::unordered_map<std::string, Value>;
  std::vector<ScopedTableType> tableStack;

 public:
  SymbolTable() {
    enterNewScope();  // global scope
  }

  std::pair<ScopedTableType::iterator, int> globalFind(const std::string& id) {
    int scopeId = tableStack.size() - 1;
    for (; scopeId >= 0; --scopeId) {
      auto& scopedSymbolTable = tableStack[scopeId];
      auto pos = scopedSymbolTable.find(id);
      if (pos != scopedSymbolTable.end()) {
        return std::make_pair(pos, scopeId);
      }
    }
    return std::make_pair(tableStack[0].end(), scopeId);
  }

  ScopedTableType::iterator currentScopeFind(const std::string& id) {
    return tableStack[tableStack.size() - 1].find(id);
  }

  ScopedTableType::iterator end() { return tableStack[0].end(); }

  void insertInCurrentScope(ScopedTableType::value_type&& node) {
    tableStack[tableStack.size() - 1].insert(std::move(node));
  }

  void enterNewScope() { tableStack.push_back(ScopedTableType()); }

  void leaveScope() { tableStack.pop_back(); }

  std::string getCurrentScopeID() {
    int id = tableStack.size() - 1;
    return id == 0 ? "" : "." + std::to_string(id);
  }

  std::string getScopeID(int id) {
    return id == 0 ? "" : "." + std::to_string(id);
  }

  const ScopedTableType& getCurrentScope() const {
    return tableStack[tableStack.size() - 1];
  }
};
}  // namespace lexy2