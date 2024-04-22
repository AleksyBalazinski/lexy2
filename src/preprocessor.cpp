#include "preprocessor.hpp"
#include <sstream>
#include <stdexcept>

namespace lexy2 {
Preprocessor::Preprocessor(const std::string& source)
    : source(source), macroDefinition(R"(^#define\s+(\w+)\s+(.+)$)") {}

std::string Preprocessor::replaceMacros(const std::string& source) {
  std::string line;
  std::smatch m;
  std::string result;
  std::istringstream in(source);
  while (std::getline(in, line)) {
    for (const auto& [macro, value] : macros) {
      std::regex bounded("\\b" + macro + "\\b");
      line = std::regex_replace(line, bounded, value);
    }
    result += line + "\n";
  }
  return result;
}

std::string Preprocessor::expandMacros() {
  auto res = loadMacros();
  auto result = replaceMacros(res);
  return result;
}

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t") {
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == std::string::npos)
    return "";  // no content

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
}

std::string Preprocessor::loadMacros() {
  std::string result;
  std::string line;
  std::smatch m;
  int lineNumber = 0;
  std::istringstream in(source);
  while (std::getline(in, line)) {
    lineNumber++;
    auto trimmed = trim(line);
    if (std::regex_match(trimmed, m, macroDefinition)) {
      auto macro = m[1].str();
      auto value = m[2].str();
      if (macros.find(macro) != macros.end()) {
        throw std::runtime_error("Redefintion of macro '" + macro +
                                 "' in line " + std::to_string(lineNumber));
      }
      macros[macro] = value;  // store line number info
      result += "\n";
    } else {
      result += line + "\n";
    }
  }
  return result;
}

}  // namespace lexy2