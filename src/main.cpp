#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <utility>
#include "Lexy2Lexer.h"
#include "Lexy2Parser.h"
#include "antlr4-runtime.h"
#include "preprocessor.hpp"
#include "translator_listener.hpp"
using namespace antlr4;

int main(int argc, const char* argv[]) {
  std::ifstream stream;
  if (argc < 2) {
    std::cerr << "Specify one input file\n";
    return 1;
  }

  const auto sourceFilename = std::string(argv[1]);
  stream.open(sourceFilename);
  if (!stream.is_open()) {
    std::cout << "File " << sourceFilename << " not found\n";
    return 1;
  }

  std::stringstream buffer;
  buffer << stream.rdbuf();
  auto bufStr = buffer.str();
  lexy2::Preprocessor preprocessor(bufStr);
  auto expanded = preprocessor.expandMacros();
  if (argc == 3 && std::strcmp(argv[2], "-E") == 0) {
    std::cout << expanded;
    return 0;
  }

  ANTLRInputStream input(expanded);
  Lexy2Lexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  Lexy2Parser parser(&tokens);

  tree::ParseTree* tree = parser.translationUnit();
  lexy2::ErrorHandler errorHandler(sourceFilename);
  lexy2::TranslatorListener translationListener(errorHandler);
  tree::ParseTreeWalker::DEFAULT.walk(&translationListener, tree);

  if (errorHandler.hasErrors()) {
    std::cerr << errorHandler.getErrors();
    return 1;
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