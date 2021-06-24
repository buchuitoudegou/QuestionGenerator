#ifndef __WHILE_EXPR_H__
#define __WHILE_EXPR_H__

#include "expression.h"

class WhileExpr: public Expr {
public:
  Expr* expr;
  WhileExpr(Expr* e);
  string stringify();
  virtual ~WhileExpr();
};

#endif