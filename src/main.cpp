#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>
#include <utility>

//using namespace antlr4;

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

  // ANTLRInputStream input(stream);
  // ExprLexer lexer(&input);
  // CommonTokenStream tokens(&lexer);
  // ExprParser parser(&tokens);

  // tree::ParseTree* tree = parser.TranslationUnit();
  // EvalListener evalListener;
  // tree::ParseTreeWalker::DEFAULT.walk(&evalListener, tree);

  // std::cout << evalListener.getResult() << '\n';
}