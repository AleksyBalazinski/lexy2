#pragma once

#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace lexy2 {
class FunctionParam;
class LLVMGenerator {
 public:
  enum class Type { I32, I8, DOUBLE, I1, FLOAT };
  enum class BinOpName { SUB, ADD, DIV, MUL, REM, CMP };
  enum class RelName { EQ, NE, GE, LE, GT, LT };

 private:
  std::string& getText() {
    if (isInFunction)
      return functionDefinitions;
    return text;
  }
  std::string text;
  std::string functionDefinitions;
  bool isInFunction = false;
  int reg = 1;

  int ifEndNumber = 0;
  int ifElseNumber = 0;
  int ifThenNumber = 0;

  int whileCondNumber = 0;
  int whileBodyNumber = 0;
  int whileEndNumber = 0;

  int arrayIndexNumber = 0;

  static std::string getOpPrefix(Type type, BinOpName op);
  static std::string getRelPrefix(Type type);
  static std::string getOperationString(BinOpName op);
  static std::string getRelName(RelName relName);
  static std::string getRelNamePrefix(RelName relName, Type type);
  static std::string getIndent();

 public:
  std::string createBinOp(BinOpName op, Type type, const std::string& arg1,
                          const std::string& arg2);

  std::string createRel(Type type, RelName relName, const std::string& arg1,
                        const std::string& arg2);

  void createAssignment(Type type, const std::string& identifier,
                        const std::string& value, bool isInternalPtr = false);

  void createDeclaration(Type type, const std::string& arg);
  void createCustomDeclaration(const std::string& typeString,
                               const std::string& arg);

  std::string createLoad(Type type, const std::string& id,
                         bool isInternalPtr = false);

  void createBranch(const std::string& cond, const std::string& ifTrue,
                    const std::string& ifFalse);

  void createBranch(const std::string& dest);
  void createLabel(const std::string& label);

  void createFunction(const std::string& functionName,
                      const std::vector<FunctionParam>& params, Type retType);
  void exitFunction();

  void createReturn(Type type, const std::string& arg);

  std::string getIfThenLabel();
  std::string getIfEndLabel();
  std::string getIfElseLabel();
  std::string getWhileCondLabel();
  std::string getWhileBodyLabel();
  std::string getWhileEndLabel();

  std::string castI32ToDouble(const std::string& id);
  std::string castI32ToFloat(const std::string& id);
  std::string castDoubleToI32(const std::string& id);
  std::string extBoolToI32(const std::string& id);
  std::string truncateI8ToI1(const std::string& val);
  std::string extI1toI8(const std::string& val);
  std::string extI32toI64(const std::string& val);
  std::string truncDoubleToFloat(const std::string& val);
  std::string castFloatToI32(const std::string& val);
  std::string extFloatToDouble(const std::string& val);

  std::string getElementPtrInBounds(const std::string& array,
                                    const std::string& element,
                                    const std::string& bounds,
                                    bool isInternalPtr);

  void printI32(const std::string& id);
  void printDouble(const std::string& id);

  std::string emitCode(const std::string& source_filename);

  static std::string getZeroLiteral(Type type);
  static bool supportsLiteralTranslation(Type from, Type to);
  static std::string getLiteral(Type from, Type to, const std::string& literal);

  static std::string getTypeString(Type type);

 private:
  std::string getRegStr() const;

  std::string getPrintfFormatStrings();

  std::string getCStdLibDeclarations();

  std::string getNumberedLabel(const char* label, int& num);
};
}  // namespace lexy2