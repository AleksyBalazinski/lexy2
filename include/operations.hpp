#pragma once

#include <stdexcept>
#include "llvm_generator.hpp"

namespace lexy2 {
enum class Relation { EQ, NEQ, LT, GT, GE, LE };

inline LLVMGenerator::RelName toLLVMRel(Relation rel) {
  switch (rel) {
    case Relation::EQ:
      return LLVMGenerator::RelName::EQ;
    case Relation::NEQ:
      return LLVMGenerator::RelName::NE;
    case Relation::GT:
      return LLVMGenerator::RelName::GT;
    case Relation::LT:
      return LLVMGenerator::RelName::LT;
    case Relation::GE:
      return LLVMGenerator::RelName::GE;
    case Relation::LE:
      return LLVMGenerator::RelName::LE;

    default:
      throw std::runtime_error("not implemented");
  }
}
}  // namespace lexy2
