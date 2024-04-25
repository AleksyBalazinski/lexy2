#include "code_gen/graph.hpp"

namespace lexy2 {
Node::Node(std::string text) : text(std::move(text)) {}

void Graph::insertNode(std::unique_ptr<Node> node) {
  nodes.push_back(std::move(node));
}
}  // namespace lexy2
