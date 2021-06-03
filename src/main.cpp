#include <iostream>
#include "expression/comp_expr.h"
#include "expression/decl_expr.h"
#include "generator/generator.h"

using std::cout;
using std::endl;

int main() {
  vector<Expr*> exprs;
  const char* v1 = "a";
  const char* v2 = "b";
  DeclExpr e1 = DeclExpr(v1, INT);
  DeclExpr e2 = DeclExpr(v2, INT);
  CompExpr e3 = CompExpr(v1, v2, PLUS);
  exprs.push_back(&e1);
  exprs.push_back(&e2);
  exprs.push_back(&e3);
  Generator g(exprs, "output.cpp");
  g.generate();
  return 0;
}