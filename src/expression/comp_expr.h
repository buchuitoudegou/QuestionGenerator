#ifndef __COMP_EXPR_H__
#define __COMP_EXPR_H__

#include "expression.h"
#include "variable.h"

class CompExpr: public Expr {
public:
  CompExpr(Variable v1, Variable v2, Variable dst, CompType type);
  CompExpr(Variable v1, Variable v2, CompType type);
  string stringify();
  virtual ~CompExpr() = default;
  Variable op1;
  Variable op2;
  Variable dst;
  bool self_comp;
  CompType comp_type;
};
#endif