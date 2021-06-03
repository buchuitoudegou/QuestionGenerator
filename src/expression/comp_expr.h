#ifndef __COMP_EXPR_H__
#define __COMP_EXPR_H__

#include "expression.h"

class CompExpr: public Expr {
public:
  CompExpr(string var_name1, string var_name2, CompType t=PLUS);
  string stringify();
  virtual ~CompExpr() = default;
  string var_name1;
  string var_name2;
  CompType comp_type;
};
#endif