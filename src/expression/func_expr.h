#ifndef __FUNC_EXPR_H__
#define __FUNC_EXPR_H__
#include "expression.h"
#include "variable.h"
#include <vector>

using std::vector;

class FuncExpr: public Expr {
public:
  FuncExpr(const char* func_name, VarType return_type, vector<VarType> args);
  string stringify();
  string get_name();
  VarType get_ret_type();
  virtual ~FuncExpr() = default;
  VarType ret_type;
  vector<VarType> args;
  string func_name;
};

class RetExpr: public Expr {
public:
  RetExpr(const char* v_name);
  string stringify();
  string v_name;
};

class OutExpr: public Expr {
public:
  OutExpr(const vector<Variable>& outs): Expr(OUTPUT), output(outs) {}
  string stringify();
  vector<Variable> output;
};
#endif