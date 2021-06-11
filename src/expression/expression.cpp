#include "expression.h"

Expr::Expr(ExprType t): type(t) {}

string stringify_vartype(VarType vt) {
  switch (vt) {
    case INT: return "int";
    case FLOAT: return "float";
  }
  return "";
}