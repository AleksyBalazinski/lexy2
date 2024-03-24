#include <cmath>
#include <iostream>
#include <stack>
#include <utility>
#include "ExprBaseListener.h"
#include "ExprLexer.h"
#include "ExprParser.h"
#include "antlr4-runtime.h"

using namespace antlr4;

class EvalListener : public ExprBaseListener {
  std::stack<double> stack;
  std::pair<double, double> popTwo() {
    double right = stack.top();
    stack.pop();
    double left = stack.top();
    stack.pop();
    return std::make_pair(left, right);
  }

 public:
  void exitNumber(ExprParser::NumberContext* ctx) override {
    auto result = std::stod(ctx->NUMBER()->toString());
    stack.push(result);
  }

  void exitNegation(ExprParser::NegationContext* ctx) override {
    auto number = stack.top();
    stack.pop();
    auto result = (-1) * number;
    stack.push(result);
  }

  void exitAdditionOrSubtraction(
      ExprParser::AdditionOrSubtractionContext* ctx) override {
    const auto [left, right] = popTwo();
    const auto& op = ctx->operator_->getText();
    double result;
    if (op == "+") {
      result = left + right;
    } else {
      result = left - right;
    }
    stack.push(result);
  }

  void exitMultiplicationOrDivision(
      ExprParser::MultiplicationOrDivisionContext* ctx) override {
    const auto [left, right] = popTwo();
    const auto& op = ctx->operator_->getText();
    double result;
    if (op == "*") {
      result = left * right;
    } else {
      result = left / right;
    }
    stack.push(result);
  }

  void exitPower(ExprParser::PowerContext* ctx) override {
    const auto [left, right] = popTwo();
    auto result = std::pow(left, right);
    stack.push(result);
  }

  double getResult() { return stack.top(); }
};

int main(int argc, const char* argv[]) {
  std::ifstream stream;
  if (argc != 2) {
    std::cerr << "Specify one input file\n";
    return 1;
  }

  stream.open(argv[1]);
  if (!stream.is_open()) {
    std::cout << "File " << argv[1] << " not found\n";
    return 1;
  }

  ANTLRInputStream input(stream);
  ExprLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  ExprParser parser(&tokens);

  tree::ParseTree* tree = parser.start();
  EvalListener evalListener;
  tree::ParseTreeWalker::DEFAULT.walk(&evalListener, tree);

  std::cout << evalListener.getResult() << '\n';
}