#include "expression.h"

Expr::Expr(ExprType t): type(t) {}

string stringify_vartype(VarType vt) {
  switch (vt) {
    case INT: return "int";
    case FLOAT: return "float";
  }
  return "";
}

string Expr::get_name() {
  return "";
}

VarType Expr::get_ret_type() {
  return VOID;
}