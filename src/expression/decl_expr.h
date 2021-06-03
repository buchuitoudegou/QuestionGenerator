#ifndef __DECL_EXPR_H__
#define __DECL_EXPR_H__
#include "expression.h"

class DeclExpr: public Expr {
public:
  DeclExpr(const char* var_name, VarType t);
  string stringify();
  virtual ~DeclExpr() = default;
  VarType var_type;
  string var_name;
};
#endif