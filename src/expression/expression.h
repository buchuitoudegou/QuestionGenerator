#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__
#include <string>

using std::string;

enum ExprType {
  DECL=1,
  COMP,
  FUNC,
  ASSIGN,
  RET
};

enum VarType {
  VOID=3,
  INT,
  FLOAT,
};

enum CompType {
  PLUS=6,
  SUB,
  TIMES,
  DIV
};

string stringify_vartype(VarType vt);

class Expr {
public:
  Expr(ExprType t);
  virtual ~Expr() = default;
  virtual string stringify() = 0;
  virtual string get_name();
  virtual VarType get_ret_type();
  ExprType type;
};

#endif