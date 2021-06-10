#include "func_expr.h"

FuncExpr::FuncExpr(const char* fn, VarType rt, vector<VarType> args):
  func_name(fn), ret_type(rt), Expr(FUNC) {}

string FuncExpr::stringify() {
  string code = "";
  switch (ret_type) {
    case INT: {
      code += "int ";
      break;
    }
    case FLOAT: {
      code += "float ";
      break;
    }
  }
  code += func_name + "() {\n";
  return code;
}

string FuncExpr::get_name() {
  return func_name;
}