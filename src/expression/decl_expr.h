#ifndef __DECL_EXPR_H__
#define __DECL_EXPR_H__
#include "expression.h"
#include "variable.h"

class DeclExpr: public Expr {
public:
  DeclExpr(const char* var_name, VarType t, const char* init_val);
  string stringify();
  virtual ~DeclExpr() = default;
  Variable var;
  string init_val;
};
#endif