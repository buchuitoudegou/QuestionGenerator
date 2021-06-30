#include "if_expr.h"

string IfExpr::stringify() {
  return "if (" + expr->stringify() + ") {";
}

IfExpr::~IfExpr() {
  if (expr) {
    delete expr;
  }
}

string ElseExpr::stringify() {
  return "else {";
}

ElseIfExpr::~ElseIfExpr() {
  if (expr) {
    delete expr;
  }
}

string ElseIfExpr::stringify() {
  return "else if (" + expr->stringify() + ") {";
}