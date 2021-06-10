#include <iostream>
#include "expression/comp_expr.h"
#include "generator/context_gen.h"
#include "generator/code_gen.h"
#include "context/condition.h"
#include <fstream>

using std::ofstream;

int main() {
  srand(1);
  int flags = LOGIC_COMP & BRANCH_SEL;
  Context* ctx = generate_context(flags);
  string code = code_generation(ctx);
  ofstream o_file;
  o_file.open("output.cpp");
  o_file << code;
  o_file.close();
  delete ctx;
  return 0;
}