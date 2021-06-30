#include "assign_expr.h"

AssignExpr::AssignExpr(Variable v, Expr* e): Expr(ASSIGN), v1(v), right_expr(e) {}

string AssignExpr::stringify() {
  return v1.v_name + " = " + right_expr->stringify();
}

AssignExpr::~AssignExpr() {
  delete right_expr;
}

BoolExpr::BoolExpr(Variable v, Expr* e, CompareType ct)
  : Expr(BOOL), v(v), right_expr(e), ct(ct) {}

string BoolExpr::stringify() {
  return v.v_name + " " + stringify_bool_type(ct) + " " + right_expr->stringify();
}

BoolExpr::~BoolExpr() {
  delete right_expr;
}

SelfCompAssignExpr::~SelfCompAssignExpr() {
  if (right_expr) {
    delete right_expr;
  }
}

string SelfCompAssignExpr::stringify() {
  return v1.v_name + " " + stringify_comp(ct) + "= " + right_expr->stringify();
}