#pragma once

#include <string>
#include <vector>

namespace lexy2 {

class ErrorHandler {
  struct Error {
    Error(int line, int column, std::string msg)
        : line(line), column(column), msg(std::move(msg)) {}
    int line;
    int column;
    std::string msg;

    std::string getErrStr() const {
      return std::to_string(line) + ":" + std::to_string(column) +
             ": \033[91merror:\033[0m " + msg + "\n\n";
    }
  };

  std::vector<Error> errors;
  std::string sourceFileName;

 public:
  ErrorHandler(std::string sourceFileName)
      : sourceFileName(std::move(sourceFileName)) {}

  void reportError(int line, int column, std::string msg) {
    errors.push_back(Error(line, column, msg));
  }

  void reportError(std::pair<int, int> pos, std::string msg) {
    auto [line, col] = pos;
    reportError(line, col, msg);
  }

  std::string getErrors() {
    std::string report;
    for (const auto& err : errors) {
      report += sourceFileName + ":" + err.getErrStr();
    }
    return report;
  }

  bool hasErrors() { return !errors.empty(); }
};
}  // namespace lexy2