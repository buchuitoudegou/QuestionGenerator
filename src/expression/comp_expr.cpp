#include "comp_expr.h"

CompExpr::CompExpr(string n1, string n2, CompType t):
  Expr(COMP), var_name1(n1), var_name2(n2), comp_type(t) {}

string CompExpr::stringify() {
  string code = var_name1;
  switch (comp_type) {
    case PLUS: {
      code += " += " + var_name2;
      break;
    }
    case SUB: {
      code += " -= " + var_name2;
      break;
    }
    case TIMES: {
      code += " *= " + var_name2;
      break;
    }
    case DIV: {
      code += " /= " + var_name2;
      break;
    }
  }
  code += ";\n";
  return code;
}