#include "code_gen/topo_sorter.hpp"

namespace lexy2 {
std::deque<Node*>& TopologicalSorter::getOrdered(const Graph& g) {
  for (const auto& node : g.nodes) {
    if (!node->visited) {
      DFSVisit(g, node.get());
    }
  }

  return ordered;
}
void TopologicalSorter::DFSVisit(const Graph& g, Node* node) {
  node->visited = true;
  for (auto v : node->neighbors) {
    if (!v->visited) {
      DFSVisit(g, v);
    }
  }
  ordered.push_front(node);
}
}  // namespace lexy2
