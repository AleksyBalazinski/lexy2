#include "type_manager.hpp"

namespace lexy2 {
LLVMGenerator::Type toLLVMType(PrimitiveType primitiveType) {
  switch (primitiveType) {
    case PrimitiveType::INT:
      return LLVMGenerator::Type::I32;
    case PrimitiveType::DOUBLE:
      return LLVMGenerator::Type::DOUBLE;
    case PrimitiveType::BOOL:
      return LLVMGenerator::Type::I1;
    case PrimitiveType::FLOAT:
      return LLVMGenerator::Type::FLOAT;
    default:
      throw std::runtime_error("Not implemented");
  }
}
}  // namespace lexy2