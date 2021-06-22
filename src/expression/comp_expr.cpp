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