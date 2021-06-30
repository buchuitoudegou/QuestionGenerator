#include "for_expr.h"

ForExpr::ForExpr(Expr* in, Expr* c, Expr* it):
  Expr(FOR), init(in), ctrl(c), iter(it) {}

string ForExpr::stringify() {
  return "for (" + init->stringify() + "; "
    + ctrl->stringify() + "; "
    + iter->stringify() + ") {";
}

ForExpr::~ForExpr() {
  delete init;
  delete ctrl;
  delete iter;
}