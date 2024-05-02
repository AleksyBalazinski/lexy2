#include <gtest/gtest.h>
#include "types/array_node.hpp"
#include "types/leaf_node.hpp"
#include "types/llvm_str_visitor.hpp"
#include "types/type.hpp"

using namespace lexy2;

TEST(LLVMStrVisitorTest, GenerationTests) {
  TypeManager typeManager;
  auto leaf =
      std::make_unique<types::LeafNode>(TypeManager::INT_TYPE_ID, typeManager);
  auto a1 = std::make_unique<types::ArrayNode>(3, std::move(leaf));
  auto a2 = std::make_unique<types::ArrayNode>(2, std::move(a1));
  auto a3 = std::make_unique<types::ArrayNode>(1, std::move(a2));
  auto type = types::Type(std::move(a3));
  types::LLVMStrVisitor visitor;
  type.applyVisitor(visitor);
  auto actual = visitor.getStr();
  const char* expected = "[1 x [2 x [3 x i32]]]";

  EXPECT_STREQ(actual.c_str(), expected);
}

TEST(TypePeelingTest, GenerationTests) {
  TypeManager typeManager;
  auto leaf =
      std::make_unique<types::LeafNode>(TypeManager::INT_TYPE_ID, typeManager);
  auto a1 = std::make_unique<types::ArrayNode>(3, std::move(leaf));
  auto a2 = std::make_unique<types::ArrayNode>(2, std::move(a1));
  auto a3 = std::make_unique<types::ArrayNode>(1, std::move(a2));
  auto type = types::Type(std::move(a3));

  auto peeled = type.getPeeledType();

  types::LLVMStrVisitor strVisitor;
  peeled->applyVisitor(strVisitor);
  auto actual = strVisitor.getStr();
  const char* expected = "[2 x [3 x i32]]";
  EXPECT_STREQ(actual.c_str(), expected);
}

TEST(FunctionTreeTest, GenerationTests) {
  TypeManager typeManager;
  auto intLeaf =
      std::make_unique<types::LeafNode>(TypeManager::INT_TYPE_ID, typeManager);
  auto arr = std::make_unique<types::ArrayNode>(3, std::move(intLeaf));
  auto boolLeaf =
      std::make_unique<types::LeafNode>(TypeManager::BOOL_TYPE_ID, typeManager);
  std::vector<std::unique_ptr<types::TypeNode>> paramTypes;
  paramTypes.push_back(std::move(arr));
  paramTypes.push_back(std::move(boolLeaf));
  auto ret = std::make_unique<types::LeafNode>(TypeManager::DOUBLE_TYPE_ID,
                                               typeManager);
  auto function = std::make_unique<types::FunctionNode>(std::move(paramTypes),
                                                        std::move(ret));

  auto functionType = types::Type(std::move(function));

  types::LLVMStrVisitor visitor;
  functionType.applyVisitor(visitor);
  auto actual = visitor.getStr();
  const char* expected = "([3 x i32], i8, ) -> double";
  EXPECT_STREQ(actual.c_str(), expected);
}