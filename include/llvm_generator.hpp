#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace lexy2 {

class LLVMGenerator {
 public:
  enum class Type { I32, I8, DOUBLE, I1 };
  enum class BinOpName { SUB, ADD, DIV, MUL, REM, CMP };
  enum class RelName { EQ, NE, GE, LE, GT, LT };

 private:
  std::string text;
  int reg = 0;
  std::string currentBasicBlock;

  int ifEndNumber = 0;
  int ifElseNumber = 0;
  int ifThenNumber = 0;

  int whileCondNumber = 0;
  int whileBodyNumber = 0;
  int whileEndNumber = 0;

  int andRhsNumber = 0;
  int andEndNumber = 0;

  static std::string getTypeString(Type type);
  static std::string getOpPrefix(Type type, BinOpName op);
  static std::string getRelPrefix(Type type);
  static std::string getOperationString(BinOpName op);
  static std::string getRelName(RelName relName);
  static std::string getRelNamePrefix(RelName relName, Type type);
  static std::string getIndent() { return "  "; }

 public:
  LLVMGenerator() { createLabel("entry"); }
  std::string createBinOp(BinOpName op, Type type, const std::string& arg1,
                          const std::string& arg2) {
    const auto regStr = getRegStr();
    text += getIndent() + regStr + " = " + getOpPrefix(type, op) +
            getOperationString(op) + " " + getTypeString(type) + " " + arg1 +
            ", " + arg2 + "\n";
    reg++;
    return regStr;
  }

  std::string createRel(Type type, RelName relName, const std::string& arg1,
                        const std::string& arg2) {
    const auto regStr = getRegStr();
    text += getIndent() + regStr + " = " + getRelPrefix(type) + "cmp" + " " +
            getRelNamePrefix(relName, type) + getRelName(relName) + " " +
            getTypeString(type) + " " + arg1 + ", " + arg2 + "\n";
    reg++;
    return regStr;
  }

  void createAssignment(Type type, const std::string& identifier,
                        const std::string& value) {
    text += getIndent() + "store " + getTypeString(type) + " " + value + ", " +
            getTypeString(type) + "* %" + identifier + "\n";
  }

  void createDeclaration(Type type, const std::string& arg) {
    text += getIndent() + "%" + arg + " = alloca " + getTypeString(type) + "\n";
  }

  std::string createLoad(Type type, const std::string& id) {
    const auto regStr = getRegStr();
    text += getIndent() + regStr + " = load " + getTypeString(type) + ", " +
            getTypeString(type) + "* %" + id + "\n";
    reg++;
    return regStr;
  }

  void createBranch(const std::string& cond, const std::string& ifTrue,
                    const std::string& ifFalse) {
    text += getIndent() + "br i1 " + cond + ", label %" + ifTrue + ", label %" +
            ifFalse + "\n";
  }

  void createBranch(const std::string& dest) {
    text += getIndent() + "br label %" + dest + "\n";
  }

  void createLabel(const std::string& label) {
    text += label + ":\n";
    currentBasicBlock = label;
  }

  std::string getCurrentBasicBlock() const { return currentBasicBlock; }

  std::string createPhi(
      std::vector<std::pair<std::string, std::string>> valueLabelPairs);

  std::string getIfThenLabel() {
    return getNumberedLabel("if.then", ifThenNumber);
  }

  std::string getIfEndLabel() {
    return getNumberedLabel("if.end", ifEndNumber);
  }

  std::string getIfElseLabel() {
    return getNumberedLabel("if.else", ifElseNumber);
  }

  std::string getWhileCondLabel() {
    return getNumberedLabel("while.cond", whileCondNumber);
  }

  std::string getWhileBodyLabel() {
    return getNumberedLabel("while.body", whileBodyNumber);
  }

  std::string getWhileEndLabel() {
    return getNumberedLabel("while.end", whileEndNumber);
  }

  std::string getAndRhsLabel() {
    return getNumberedLabel("and.rhs", andRhsNumber);
  }

  std::string getAndEndLabel() {
    return getNumberedLabel("and.end", andEndNumber);
  }

  std::string castI32ToDouble(const std::string& id);
  std::string castDoubleToI32(const std::string& id);
  std::string castBoolToI32(const std::string& id);
  std::string truncateI8ToI1(const std::string& val);
  std::string castI1toI8(const std::string& val);

  void printI32(const std::string& id);
  void printDouble(const std::string& id);

  std::string emitCode(const std::string& source_filename);

  static std::string getZeroLiteral(Type type);

 private:
  std::string getRegStr() const { return "%" + std::to_string(reg); }

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

  std::string getNumberedLabel(const char* label, int& num) {
    auto suffix = num == 0 ? "" : std::to_string(num);
    ++num;
    return label + suffix;
  }
};
}  // namespace lexy2