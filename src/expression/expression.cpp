#include "expression.h"

Expr::Expr(ExprType t): type(t) {}

string stringify_vartype(VarType vt) {
  switch (vt) {
    case INT: return "int";
    case FLOAT: return "float";
  }
  return "";
}
string stringify_comp(CompType ct) {
  switch (ct) {
    case PLUS: return "+";
    case SUB: return "-";
    case TIMES: return "*";
    case DIV: return "/";
  }
  return "";
}

string stringify_bool_type(CompareType ct) {
  switch (ct) {
    case EQUAL: return "==";
    case LARGER: return ">";
    case LESS: return "<";
    case NO_LARGER: return ">=";
    case NO_LESS: return "<=";
  }
  return "";
}

string Expr::get_name() {
  return "";
}

VarType Expr::get_ret_type() {
  return VOID;
}