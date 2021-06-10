#include "decl_expr.h"

DeclExpr::DeclExpr(const char* n, VarType t, const char* v):
  Expr(DECL), var(n, t), init_val(v) {}

string DeclExpr::stringify() {
  string type = var.v_type == INT ? "int" : "float";
  return type + " " + var.v_name + " = " + init_val + ";\n";
}