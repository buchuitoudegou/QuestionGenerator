#ifndef __ASSIGN_EXPR_H__
#define __ASSIGN_EXPR_H__

#include "expression.h"
#include <memory>

using std::shared_ptr;

// variable = Expr
class AssignExpr: public Expr {
public:
string v_name;
shared_ptr<Expr> right_expr;
string stringify();
};

#endif