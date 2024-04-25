#include "code_gen/topo_sorter.hpp"
#include <gtest/gtest.h>
#include <algorithm>
#include "code_gen/graph.hpp"

using namespace lexy2;

TEST(TopoSorterTest, GraphTests) {
  auto f1 = std::make_unique<Node>("f1");
  auto f2 = std::make_unique<Node>("f2");
  auto f3 = std::make_unique<Node>("f3");
  auto f4 = std::make_unique<Node>("f4");
  auto f5 = std::make_unique<Node>("f5");
  auto f6 = std::make_unique<Node>("f6");

  f1->neighbors.push_back(f2.get());
  f1->neighbors.push_back(f4.get());
  f2->neighbors.push_back(f3.get());
  f4->neighbors.push_back(f2.get());
  f5->neighbors.push_back(f1.get());
  f5->neighbors.push_back(f6.get());

  Graph graph;
  graph.insertNode(std::move(f1));
  graph.insertNode(std::move(f2));
  graph.insertNode(std::move(f3));
  graph.insertNode(std::move(f4));
  graph.insertNode(std::move(f5));
  graph.insertNode(std::move(f6));

  TopologicalSorter sorter;
  auto& ordered = sorter.getOrdered(graph);

  std::vector<Node*> seen;
  for (auto elem : ordered) {
    for (auto neighbor : elem->neighbors) {
      EXPECT_TRUE(std::find(seen.begin(), seen.end(), neighbor) == seen.end());
    }
    seen.push_back(elem);
  }
}