#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__
#include <string>

using std::string;

enum ExprType {
  DECL=1,
  COMP
};

enum VarType {
  INT=3,
  FLOAT,
};

enum CompType {
  PLUS=5,
  SUB,
  TIMES,
  DIV
};

class Expr {
public:
  Expr(ExprType t);
  virtual ~Expr() = default;
  virtual string stringify() = 0;
  ExprType type;
};

#endif