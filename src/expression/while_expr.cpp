#include "while_expr.h"

WhileExpr::WhileExpr(Expr* e): Expr(WHILE), expr(e) {}

WhileExpr::~WhileExpr() {
  delete expr;
}

string WhileExpr::stringify() {
  return "while(" + expr->stringify() + ")" + " {\n";
}