#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__
#include <string>

using std::string;

enum ExprType {
  DECL=1,
  COMP,
  FUNC,
  ASSIGN,
  BOOL,
  RET,
  VAR,
  WHILE,
  FOR,
  SEL,
};

enum VarType {
  VOID=3,
  INT,
  FLOAT,
};

enum CompareType {
  EQUAL=0, // ==
  LARGER, // >
  LESS, // <
  NO_LESS, // >=
  NO_LARGER, // <=
};

enum CompType {
  PLUS=6,
  SUB,
  TIMES,
  DIV,
  MOD
};

string stringify_vartype(VarType vt);
string stringify_comp(CompType ct);
string stringify_bool_type(CompareType ct);
bool need_semicolon(ExprType type);

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