#pragma once
#include <istream>
#include <regex>
#include <string>
#include <unordered_map>

namespace lexy2 {
class Preprocessor {
 private:
  const std::string& source;
  std::string result;
  std::unordered_map<std::string, std::string> macros;
  const std::regex macroDefinition;

 public:
  Preprocessor(const std::string& source);
  std::string expandMacros();

 private:
  std::string loadMacros();
  std::string replaceMacros(const std::string& source);
};
}  // namespace lexy2
