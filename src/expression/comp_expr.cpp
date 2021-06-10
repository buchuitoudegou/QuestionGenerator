#include "comp_expr.h"

CompExpr::CompExpr(Variable v1, Variable v2, Variable v3, CompType t):
  Expr(COMP), op1(v1), op2(v2), dst(v3), comp_type(t) {
    self_comp = false;
  }

CompExpr::CompExpr(Variable v1, Variable v2, CompType t):
  Expr(COMP), op1(v1), op2(v2), dst(v2), comp_type(t) {
    self_comp = true;
  }

string CompExpr::stringify() {
  string code = "";
  switch (comp_type) {
    case PLUS: {
      if (self_comp) {
        code += op2.v_name + " += " + op1.v_name;
      } else {
        code += dst.v_name + " = " + op1.v_name + " + " + op2.v_name;
      }
      break;
    }
    case SUB: {
      if (self_comp) {
        code += op2.v_name + " -= " + op1.v_name;
      } else {
        code += dst.v_name + " = " + op1.v_name + " - " + op2.v_name;
      }
      break;
    }
    case TIMES: {
      if (self_comp) {
        code += op2.v_name + " *= " + op1.v_name;
      } else {
        code += dst.v_name + " = " + op1.v_name + " * " + op2.v_name;
      }
      break;
    }
    case DIV: {
      if (self_comp) {
        code += op2.v_name + " /= " + op1.v_name;
      } else {
        code += dst.v_name + " = " + op1.v_name + " / " + op2.v_name;
      }
      break;
    }
  }
  code += ";\n";
  return code;
}