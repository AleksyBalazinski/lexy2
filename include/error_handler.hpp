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

    std::string getErrStr() const;
  };

  std::vector<Error> errors;
  std::string sourceFileName;

 public:
  ErrorHandler(std::string sourceFileName)
      : sourceFileName(std::move(sourceFileName)) {}

  void reportError(int line, int column, std::string msg);

  void reportError(std::pair<int, int> pos, std::string msg);

  std::string getErrors();

  bool hasErrors();
};
}  // namespace lexy2