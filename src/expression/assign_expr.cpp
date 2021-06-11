#include "assign_expr.h"

string AssignExpr::stringify() {
  return v_name + " = " + right_expr->stringify();
}