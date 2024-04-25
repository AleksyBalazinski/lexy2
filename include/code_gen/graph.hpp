#pragma once

#include <memory>
#include <string>
#include <vector>

namespace lexy2 {
struct Node {
  std::vector<Node*> neighbors;
  std::string text;
  bool visited = false;
  int ttl;

  Node(std::string text = "");
};

struct Graph {
  std::vector<std::unique_ptr<Node>> nodes;

  void insertNode(std::unique_ptr<Node> node);
};
}  // namespace lexy2
