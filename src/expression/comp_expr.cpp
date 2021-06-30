#include "comp_expr.h"

VarExpr::VarExpr(Variable v): Expr(VAR), var(v) {}

string VarExpr::stringify() {
  return var.v_name;
}

CompExpr::CompExpr(Expr* _e1, Expr* _e2, CompType c):
  Expr(COMP), e1(_e1), e2(_e2), comp_type(c) {}

CompExpr::~CompExpr() {
  delete e1;
  delete e2;
}

string CompExpr::stringify() {
  return e1->stringify() + " "
    + stringify_comp(comp_type) + " "
    + e2->stringify();
}

SelfCompExpr::SelfCompExpr(Variable v, CompType t):
  Expr(COMP), v(v), comp_type(t), is_back(true) {}

SelfCompExpr::SelfCompExpr(Variable v, CompType t, bool is_back):
  Expr(COMP), v(v), comp_type(t), is_back(is_back) {}

string SelfCompExpr::stringify() {
  if (is_back) {
    return v.v_name + stringify_comp(comp_type) + stringify_comp(comp_type);
  } else {
    return stringify_comp(comp_type) + stringify_comp(comp_type) + v.v_name;
  }
}

ConstExpr::ConstExpr(const char* c): Expr(VAR), c(c) {}

string ConstExpr::stringify() {
  return c;
}