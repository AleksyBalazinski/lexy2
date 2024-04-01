#pragma once

#include <string>
#include <string_view>

namespace lexy2 {

class LLVMGenerator {
  std::string text;
  int reg = 1;

 public:
  void declareI32(const std::string& id);
  void declareDouble(const std::string& id);

  void assignI32(const std::string& id, const std::string& value);
  void assignDouble(const std::string& id, const std::string& value);

  std::string loadI32(const std::string& id);
  std::string loadDouble(const std::string& id);

  std::string addI32(const std::string& val1, const std::string& val2);
  std::string addDouble(const std::string& val1, const std::string& val2);

  std::string subI32(const std::string& val1, const std::string& val2);
  std::string subDouble(const std::string& val1, const std::string& val2);

  std::string mulI32(const std::string& val1, const std::string& val2);
  std::string mulDouble(const std::string& val1, const std::string& val2);

  std::string divI32(const std::string& val1, const std::string& val2);
  std::string divDouble(const std::string& val1, const std::string& val2);

  std::string remI32(const std::string& val1, const std::string& val2);

  std::string castI32ToDouble(const std::string& id);
  std::string castDoubleToI32(const std::string& id);

  void printI32(const std::string& id);
  void printDouble(const std::string& id);

  std::string emitCode(const std::string& source_filename);

 private:
  std::string getRegStr() { return "%" + std::to_string(reg); }

  std::string getPrintfFormatStrings() {
    std::string formatInt =
        R"(@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1)";
    std::string formatDouble =
        R"(@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1)";
    return formatInt + "\n" + formatDouble + "\n";
  }

  std::string getCStdLibDeclarations() {
    std::string declarePrintf = "declare i32 @printf(ptr, ...)";
    return declarePrintf + "\n";
  }
};
}  // namespace lexy2