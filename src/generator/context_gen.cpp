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
    Expr* comp = context->def_simple_comp(INT, INT, PLUS, "foo");
    Variable v = context->get_var(INT, "foo");
    AssignExpr* e = new AssignExpr(v, comp);
    context->insert_func_norm_expr("foo", e);
    context->insert_func_ret_expr("foo");
  }
  context->define_main();
  return context;
}