#include "decl_expr.h"

DeclExpr::DeclExpr(const char* n, VarType t):
  Expr(DECL), var_type(t), var_name(n) {}

string DeclExpr::stringify() {
  string type = var_type == INT ? "int" : "float";
  return type + " " + var_name + ";\n";
}