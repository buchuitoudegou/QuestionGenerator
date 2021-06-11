#include "context_gen.h"

Context* generate_context(uint16_t flags) {
  Context* context = new Context();
  // todo: generate context that fulfill the condition flag
  if (flags & FUNC_DEF) {
    vector<VarType> args; // the args can vary
    args.push_back(INT);
    args.push_back(INT);
    const char* func_name = "foo"; // supposed to be randomly generated
    context->define_udf(func_name, args, INT);
    context->insert_func_arithmetic_comp(false, PLUS, INT, "foo");
  }
  context->define_main();
  context->insert_func_arithmetic_comp(true, PLUS, INT, "main");
  return context;
}