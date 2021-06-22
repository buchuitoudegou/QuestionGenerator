#ifndef __COMP_EXPR_H__
#define __COMP_EXPR_H__

#include "expression.h"
#include "variable.h"

class VarExpr: public Expr {
public:
  VarExpr(Variable v);
  string stringify();
  virtual ~VarExpr() = default;
  Variable var;
};

class CompExpr: public Expr {
public:
  CompExpr(Expr* e1, Expr* e2, CompType t);
  string stringify();
  virtual ~CompExpr();
  Expr* e1;
  Expr* e2;
  CompType comp_type;
};
#endif