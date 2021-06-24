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

SelfCompExpr::SelfCompExpr(Variable v, CompType t, Expr* e1):
  Expr(COMP), v(v), comp_type(t), e(e1) {
    constant = "";
  }

SelfCompExpr::SelfCompExpr(Variable v, CompType t, const char* c):
  Expr(COMP), v(v), comp_type(t), constant(c) {
    e = NULL;
  }

string SelfCompExpr::stringify() {
  string base = v.v_name + " " + stringify_comp(comp_type) + "= ";
  if (e) {
    return base + e->stringify() + ";\n";
  } else {
    return base + constant + ";\n";
  }
}

SelfCompExpr::~SelfCompExpr() {
  if (e) {
    delete e;
  }
}

ConstExpr::ConstExpr(const char* c): Expr(VAR), c(c) {}

string ConstExpr::stringify() {
  return c;
}