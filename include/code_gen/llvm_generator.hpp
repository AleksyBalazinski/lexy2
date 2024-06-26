#pragma once

#include <optional>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
#include "code_gen/graph.hpp"
#include "function_param.hpp"
#include "operations.hpp"
#include "struct.hpp"
#include "types/type.hpp"

namespace lexy2 {

class LLVMGenerator {
 public:
  enum class Type { I32, I8, DOUBLE, I1, FLOAT };
  enum class BinOpName { SUB, ADD, DIV, MUL, REM, CMP, AND, OR, XOR };

 private:
  std::string& getText() {
    if (!activeFunctions.empty())
      return activeFunctions.top()->text;
    return text;
  }
  std::string& getHeader() { return activeFunctions.top()->header; }
  std::string text;
  std::string structsDeclarations;
  std::stack<Node*> functionDefs;
  std::stack<Node*> activeFunctions;
  Graph graph;
  int reg = 1;

  int ifEndNumber = 0;
  int ifElseNumber = 0;
  int ifThenNumber = 0;

  int whileCondNumber = 0;
  int whileBodyNumber = 0;
  int whileEndNumber = 0;

  int arrayIndexNumber = 0;
  int structIndexNumber = 0;

  int callRegisterNumber = 0;

  static std::string getOpPrefix(const types::Type& type, BinOpName op);
  static std::string getRelPrefix(const types::Type& type);
  static std::string getOperationString(BinOpName op);
  static std::string getRelName(Relation relName);
  static std::string getRelNamePrefix(Relation relName,
                                      const types::Type& type);
  static std::string getIndent();

 public:
  LLVMGenerator();

  std::string createBinOp(BinOpName op, const types::Type& type,
                          const std::string& arg1, const std::string& arg2);

  std::string createRel(const types::Type& type, Relation relName,
                        const std::string& arg1, const std::string& arg2);

  void createAssignment(const types::Type& type, const std::string& identifier,
                        const std::string& value, bool boolAsI1,
                        bool isInternalPtr = false);

  void createDeclaration(const types::Type& type, const std::string& arg,
                         bool boolAsI1);

  std::string createLoad(const types::Type& type, const std::string& id,
                         bool boolAsI1, bool isInternalPtr = false);

  void createBranch(const std::string& cond, const std::string& ifTrue,
                    const std::string& ifFalse);

  void createBranch(const std::string& dest);
  void createLabel(const std::string& label);

  void createFunction(const std::string& functionName,
                      const std::vector<FunctionParam>& params,
                      const types::Type& retType);
  void exitFunction();
  void enterFunction();

  void createStruct(const Struct& struct_);

  void createReturn(const types::Type& type, const std::string& arg);
  std::string createCall(const std::string& functionName,
                         const std::vector<FunctionParam>& args,
                         const types::Type& retType);

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
  std::string truncI32ToBool(const std::string& val);
  std::string extI1toI8(const std::string& val);
  std::string extI32toI64(const std::string& val);
  std::string truncDoubleToFloat(const std::string& val);
  std::string castFloatToI32(const std::string& val);
  std::string extFloatToDouble(const std::string& val);

  std::string getArrElementPtrInBounds(const std::string& array,
                                       const std::string& element,
                                       const std::string& bounds,
                                       bool isInternalPtr);

  std::string getStructElementPtrInBounds(const std::string& structName,
                                          const std::string& element,
                                          const std::string& bounds,
                                          bool isInternalPtr = false);

  void printI32(const std::string& id);
  void printDouble(const std::string& id);

  void read(const std::string& id, const types::Type& type,
            bool isInternalPtr = false);

  std::string emitCode(const std::string& source_filename);

  static std::string getZeroLiteral(const types::Type& type);

  static bool supportsLiteralTranslation(const types::Type& from,
                                         const types::Type& to);

  static std::string getLiteral(const types::Type& from, const types::Type& to,
                                const std::string& literal);

 private:
  std::string getRegStr() const;

  std::string getPrintfFormatStrings();

  std::string getCStdLibDeclarations();

  std::string getNumberedLabel(const char* label, int& num);

  std::string formatIntNewLine =
      R"(@formatIntNewLine = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1)";
  std::string formatFloatNewLine =
      R"(@formatFloatNewLine = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1)";
  std::string formatInt =
      R"(@formatInt = private unnamed_addr constant [3 x i8] c"%d\00", align 1)";
  std::string formatDouble =
      R"(@formatDouble = private unnamed_addr constant [4 x i8] c"%lf\00", align 1)";
  std::string formatFloat =
      R"(@formatFloat = private unnamed_addr constant [3 x i8] c"%f\00", align 1)";

  std::vector<std::string> formatStrs;
};
}  // namespace lexy2