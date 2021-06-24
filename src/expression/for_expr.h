#ifndef __FOR_EXPR_H__
#define __FOR_EXPR_H__

#include "expression.h"

class ForExpr: public Expr {
public:
  ForExpr(Expr* init, Expr* ctrl, Expr* iterate);
  Expr* init, *ctrl, *iter;
  string stringify();
  virtual ~ForExpr();
};

#endif