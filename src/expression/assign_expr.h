#ifndef __ASSIGN_EXPR_H__
#define __ASSIGN_EXPR_H__

#include "expression.h"
#include "variable.h"

// variable = Expr
class AssignExpr: public Expr {
public:
  Variable v1;
  Expr* right_expr;
  string stringify();
  AssignExpr(Variable v, Expr* e);
  virtual ~AssignExpr();
};

class SelfCompAssignExpr: public Expr {
public:
  Variable v1;
  Expr* right_expr;
  CompType ct;
  string stringify();
  SelfCompAssignExpr(Variable v, CompType ct, Expr* e):
  Expr(ASSIGN), v1(v), ct(ct), right_expr(e) {};
  virtual ~SelfCompAssignExpr();
};

// variable > / < / == Expr
class BoolExpr: public Expr {
public:
  Variable v;
  CompareType ct;
  Expr* right_expr;
  string stringify();
  BoolExpr(Variable v, Expr* e, CompareType ct);
  virtual ~BoolExpr();
};

#endif