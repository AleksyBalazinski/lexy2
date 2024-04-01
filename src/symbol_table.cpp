#include "symbol_table.hpp"

namespace lexy2 {
SymbolTable::SymbolTable() {
  enterNewScope();  // global scope
}

using ScopedTableType = std::unordered_map<std::string, Value>;

std::pair<ScopedTableType::iterator, int> SymbolTable::globalFind(
    const std::string& id) {
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

ScopedTableType::iterator SymbolTable::currentScopeFind(const std::string& id) {
  return tableStack[tableStack.size() - 1].find(id);
}

ScopedTableType::iterator SymbolTable::end() {
  return tableStack[0].end();
}

void SymbolTable::insertInCurrentScope(ScopedTableType::value_type&& node) {
  tableStack[tableStack.size() - 1].insert(std::move(node));
}

void SymbolTable::enterNewScope() {
  tableStack.push_back(ScopedTableType());
}

void SymbolTable::leaveScope() {
  tableStack.pop_back();
}

std::string SymbolTable::getCurrentScopeID() {
  int id = tableStack.size() - 1;
  return id == 0 ? "" : "." + std::to_string(id);
}

std::string SymbolTable::getScopeID(int id) {
  return id == 0 ? "" : "." + std::to_string(id);
}

const ScopedTableType& SymbolTable::getCurrentScope() const {
  return tableStack[tableStack.size() - 1];
}

}  // namespace lexy2