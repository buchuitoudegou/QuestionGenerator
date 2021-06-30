#include <iostream>
#include "expression/comp_expr.h"
#include "generator/context_gen.h"
#include "generator/code_gen.h"
#include "context/condition.h"
#include <fstream>

using std::ofstream;

int main() {
  srand(time(NULL));
  uint16_t flags = CMP_ASGN | WHILE | IF | VC_CND | VV_CND | CPLX_CND | NST_BLK;
  Context* ctx = generate_context(flags, 3, 10, 1.0);
  string code = code_generation(ctx);
  ofstream o_file;
  o_file.open("output.cpp");
  o_file << code;
  o_file.close();
  delete ctx;
  return 0;
}