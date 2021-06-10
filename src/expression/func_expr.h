#ifndef __FUNC_EXPR_H__
#define __FUNC_EXPR_H__
#include "expression.h"
#include <vector>

using std::vector;

class FuncExpr: public Expr {
public:
  FuncExpr(const char* func_name, VarType return_type, vector<VarType> args);
  string stringify();
  virtual ~FuncExpr() = default;
  VarType ret_type;
  string func_name;
};
#endif