#include "error_handler.hpp"

namespace lexy2 {
void ErrorHandler::reportError(int line, int column, std::string msg) {
  errors.push_back(Error(line, column, msg));
}

void ErrorHandler::reportError(std::pair<int, int> pos, std::string msg) {
  auto [line, col] = pos;
  reportError(line, col, msg);
}

std::string ErrorHandler::getErrors() {
  std::string report;
  for (const auto& err : errors) {
    report += sourceFileName + ":" + err.getErrStr();
  }
  return report;
}

bool ErrorHandler::hasErrors() {
  return !errors.empty();
}

std::string ErrorHandler::Error::getErrStr() const {
  return std::to_string(line) + ":" + std::to_string(column) +
         ": \033[91merror:\033[0m " + msg + "\n\n";
}

}  // namespace lexy2
