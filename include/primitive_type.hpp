#pragma once
#include "llvm_generator.hpp"

namespace lexy2 {
enum class PrimitiveType { INT, DOUBLE, BOOL, FLOAT };
LLVMGenerator::Type toLLVMType(PrimitiveType primitiveType);
}  // namespace lexy2
