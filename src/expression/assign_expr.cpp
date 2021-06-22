#include "assign_expr.h"

AssignExpr::AssignExpr(Variable v, Expr* e): Expr(ASSIGN), v1(v), right_expr(e) {}

string AssignExpr::stringify() {
  return v1.v_name + " = " + right_expr->stringify() + ";\n";
}

AssignExpr::~AssignExpr() {
  delete right_expr;
}