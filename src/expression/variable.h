#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>
#include "expression.h"

struct Variable {
  string v_name;
  VarType v_type;
  Variable(const char* n, VarType t): v_name(n), v_type(t) {}
};

#endif