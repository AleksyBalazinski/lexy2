#include "symbol_table.hpp"

namespace lexy2 {
inline int SymbolTable::getCurrentDepth() const {
  return tableStack.size() - 1;
}

SymbolTable::SymbolTable() {
  enterNewScope();  // global scope
  scopeIDBuilder.reserve(5);
  scopeIDBuilder.push_back(0);
}

using ScopedTableType = std::unordered_map<std::string, Value>;

std::pair<ScopedTableType::iterator, int> SymbolTable::globalFind(
    const std::string& id) {
  int depth = getCurrentDepth();
  for (; depth >= 0; --depth) {
    auto& scopedSymbolTable = tableStack[depth];
    auto pos = scopedSymbolTable.find(id);
    if (pos != scopedSymbolTable.end()) {
      return std::make_pair(pos, depth);
    }
  }
  return std::make_pair(tableStack[0].end(), depth);
}

ScopedTableType::iterator SymbolTable::currentScopeFind(const std::string& id) {
  return tableStack[getCurrentDepth()].find(id);
}

ScopedTableType::iterator SymbolTable::end() {
  return tableStack[0].end();
}

void SymbolTable::insertInCurrentScope(ScopedTableType::value_type&& node) {
  tableStack[getCurrentDepth()].insert(std::move(node));
}

void SymbolTable::enterNewScope() {
  tableStack.push_back(ScopedTableType());

  int depth = getCurrentDepth();
  if (depth != 0) {
    if (scopeIDBuilder.size() < depth + 1) {
      scopeIDBuilder.push_back(1);
    } else {
      ++scopeIDBuilder[depth];
    }
  }
}

void SymbolTable::leaveScope() {
  tableStack.pop_back();

  int depth = getCurrentDepth();
  if (scopeIDBuilder.size() > depth + 2) {
    scopeIDBuilder[depth + 2] = 0;
  }
}

std::string SymbolTable::getCurrentScopeID() {
  int depth = getCurrentDepth();
  return getScopeID(depth);
}

std::string SymbolTable::getScopeID(int depth) {
  if (depth == 0) {
    return "";
  }
  std::string scopeID;
  for (int i = 1; i <= depth; i++) {
    int mark = scopeIDBuilder[i];
    scopeID.append("." + std::to_string(mark));
  }
  return scopeID;
}

const ScopedTableType& SymbolTable::getCurrentScope() const {
  return tableStack[getCurrentDepth()];
}

}  // namespace lexy2