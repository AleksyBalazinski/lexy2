#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>
#include <utility>
#include "Lexy2Lexer.h"
#include "Lexy2Parser.h"
#include "antlr4-runtime.h"
#include "translator_listener.hpp"
using namespace antlr4;

int main(int argc, const char* argv[]) {
  std::ifstream stream;
  if (argc != 2) {
    std::cerr << "Specify one input file\n";
    return 1;
  }

  const auto sourceFilename = std::string(argv[1]);
  stream.open(sourceFilename);
  if (!stream.is_open()) {
    std::cout << "File " << sourceFilename << " not found\n";
    return 1;
  }

  ANTLRInputStream input(stream);
  Lexy2Lexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  Lexy2Parser parser(&tokens);

  tree::ParseTree* tree = parser.translationUnit();
  ErrorHandler errorHandler(sourceFilename);
  TranslatorListener translationListener(errorHandler);
  tree::ParseTreeWalker::DEFAULT.walk(&translationListener, tree);

  if (errorHandler.hasErrors()) {
    std::cerr << errorHandler.getErrors();
    return 2;
  }

  const auto llvmCode = translationListener.getCode(sourceFilename);
  const auto llvmIRName =
      sourceFilename.substr(0, sourceFilename.find_last_of('.')) + ".ll";

  std::ofstream out(llvmIRName);
  if (out.is_open()) {
    out << llvmCode;
  } else {
    std::cerr << "Couldn't open " << llvmIRName << " for writing.\n";
    return 1;
  }
}