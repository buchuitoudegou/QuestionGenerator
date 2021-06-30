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

class ConstExpr: public Expr {
public:
  ConstExpr(const char* c);
  string stringify();
  string c;
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

class SelfCompExpr: public Expr {
public:
  SelfCompExpr(Variable v, CompType t);
  SelfCompExpr(Variable v, CompType t, bool is_back);
  virtual ~SelfCompExpr()=default;
  string stringify();
  bool is_back;
  CompType comp_type;
  Variable v;
};
#endif