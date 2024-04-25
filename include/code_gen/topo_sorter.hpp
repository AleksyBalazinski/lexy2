#pragma once
#include <deque>
#include "code_gen/graph.hpp"

namespace lexy2 {
class TopologicalSorter {
  std::deque<Node*> ordered;

 public:
  std::deque<Node*>& getOrdered(const Graph& g);

 private:
  void DFSVisit(const Graph& g, Node* node);
};
}  // namespace lexy2
