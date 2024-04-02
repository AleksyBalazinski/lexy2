#include "llvm_generator.hpp"

namespace lexy2 {
void LLVMGenerator::declareI32(const std::string& id) {
  text += "%" + id + " = alloca i32\n";
}

void LLVMGenerator::declareDouble(const std::string& id) {
  text += "%" + id + " = alloca double\n";
}

void LLVMGenerator::declareI8(const std::string& id) {
  text += "%" + id + " = alloca i8\n";
}

void LLVMGenerator::assignI32(const std::string& id, const std::string& value) {
  text += "store i32 " + value + ", i32* %" + id + "\n";
}

void LLVMGenerator::assignDouble(const std::string& id,
                                 const std::string& value) {
  text += "store double " + value + ", double* %" + id + "\n";
}

void LLVMGenerator::assignI8(const std::string& id, const std::string& value) {
  text += "store i8 " + value + ", i8* %" + id + "\n";
}

std::string LLVMGenerator::loadI32(const std::string& id) {
  auto regStr = getRegStr();
  text += regStr + " = load i32, i32* %" + id + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::loadDouble(const std::string& id) {
  auto regStr = getRegStr();
  text += regStr + " = load double, double* %" + id + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::loadI8(const std::string& id) {
  auto regStr = getRegStr();
  text += regStr + " = load i8, i8* %" + id + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::addI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = add i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::addDouble(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fadd double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::subI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = sub i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::subDouble(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fsub double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::mulI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = mul i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::mulDouble(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fmul double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::divI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = sdiv i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::divDouble(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fdiv double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::remI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = srem i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castI32ToDouble(const std::string& id) {
  const auto regStr = getRegStr();
  text += regStr + " = sitofp i32 " + id + " to double\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castDoubleToI32(const std::string& id) {
  const auto regStr = getRegStr();
  text += regStr + " = fptosi double " + id + " to i32\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castBoolToI32(const std::string& id) {
  const auto regStr = getRegStr();
  text += regStr + " = zext i1 " + id + " to i32\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::truncateI8ToI1(const std::string& val) {
  const auto regStr = getRegStr();
  text += regStr + " = trunc i8 " + val + " to i1\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castI1toI8(const std::string& val) {
  const auto regStr = getRegStr();
  text += regStr + " = zext i1 " + val + " to i8\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpEqI32(const std::string& val1,
                                    const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = icmp eq i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpEqDouble(const std::string& val1,
                                       const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fcmp oeq double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpNeqI32(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = icmp ne i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpNeqDouble(const std::string& val1,
                                        const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fcmp one double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpGeqI32(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = icmp sge i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpGeqDouble(const std::string& val1,
                                        const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fcmp oge double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpGreaterThanI32(const std::string& val1,
                                             const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = icmp sgt i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpGreaterThanDouble(const std::string& val1,
                                                const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fcmp ogt double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpLeqI32(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = icmp sle i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpLeqDouble(const std::string& val1,
                                        const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fcmp ole double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpLessThanI32(const std::string& val1,
                                          const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = icmp slt i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::cmpLessThanDouble(const std::string& val1,
                                             const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fcmp olt double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

void LLVMGenerator::printI32(const std::string& id) {
  text += "call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef " +
          id + ")\n";
  reg++;
}

void LLVMGenerator::printDouble(const std::string& id) {
  text +=
      "call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef " +
      id + ")\n";
  reg++;
}

std::string LLVMGenerator::emitCode(const std::string& source_filename) {
  std::string code;
  code += "source_filename = \"" + source_filename + "\"\n";
  code +=
      "target triple = \"x86_64-w64-windows-gnu\"\n\n";  // TODO: obtain this somehow
  code += getPrintfFormatStrings() + "\n";
  code += getCStdLibDeclarations() + "\n";
  code += "define dso_local i32 @main() #0 {\n";
  code += text;
  code += "ret i32 0\n}\n";
  return code;
}
}  // namespace lexy2