#ifndef __IF_EXPR_H__
#define __IF_EXPR_H__

#include "expression.h"

class IfExpr: public Expr {
public:
  IfExpr(Expr* e): expr(e), Expr(SEL) {}
  virtual ~IfExpr();
  string stringify();
  Expr* expr;
};

class ElseExpr: public Expr {
public:
  ElseExpr(): Expr(SEL) {};
  string stringify();
};

class ElseIfExpr: public Expr {
public:
  ElseIfExpr(Expr* e): Expr(SEL), expr(e) {}
  Expr* expr;
  virtual ~ElseIfExpr();
  string stringify();
};

#endif