#include "func_expr.h"

FuncExpr::FuncExpr(const char* fn, VarType rt, vector<VarType> a):
  Expr(FUNC), func_name(fn), ret_type(rt), args(a) {}

string FuncExpr::stringify() {
  string code = "";
  code += stringify_vartype(ret_type) + " ";
  string args_list = "(";
  for (int i = 0; i < args.size(); ++i) {
    char temp = 'a' + i;
    string arg_name;
    arg_name.push_back(temp);
    if (i != 0) {
      args_list += ",";
    }
    args_list += stringify_vartype(args[i]) + " " + arg_name;
  }
  args_list += ")";
  code += func_name + args_list + " {";
  return code;
}

string FuncExpr::get_name() {
  return func_name;
}

VarType FuncExpr::get_ret_type() {
  return this->ret_type;
}

RetExpr::RetExpr(const char* n): v_name(n), Expr(RET) {}

string RetExpr::stringify() {
  return "return " + v_name;
}

string OutExpr::stringify() {
  string code("printf(\""), params;
  for (auto& var: output) {
    if (params.size() != 0) {
      params.push_back(',');
    }
    params += var.v_name;
    if (var.v_type == INT) {
      code += "%d ";
    } else {
      code += "%f ";
    }
  }
  code.pop_back();
  code += "\\";
  code += "n";
  code += "\",";
  params += ")";
  return code + params;
}