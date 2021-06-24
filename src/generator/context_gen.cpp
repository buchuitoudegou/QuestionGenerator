#include "context_gen.h"

Context* generate_context(uint16_t flags) {
  Context* context = new Context();
  // todo: generate context that fulfill the condition flag
  if (flags & FUNC_DEF) {
    vector<VarType> args; // the args can vary
    int arg_num = rand() % 5;
    printf("function arg num: %d\n", arg_num);
    for (int i = 0; i < arg_num; ++i) {
      VarType type = INT;
      int r = rand() % 2;
      // todo: randomize arg type
      args.push_back(type);
    }
    const char* func_name = "foo"; // supposed to be randomly generated
    context->define_udf(func_name, args, INT); // ret type should be randomized
    decl_var(context, INT, "foo");
    if (flags & FUNC_ARTH) {
      gen_arth(context, INT, "foo");
    }
    context->insert_func_ret_expr("foo");
  }
  context->define_main();
  if (flags & ARTH) {
    decl_var(context, INT, "main");
    decl_var(context, INT, "main");
    gen_arth(context, INT, "main");
  }
  if (flags & WHILE_LOOP) {
    gen_while(context, "main", 3);
  }
  return context;
}

bool gen_arth(Context* ctx, VarType vt, string func_name)  {
  int arg_num = rand() % 2 + 2;
  vector<VarExpr*> vs;
  vector<CompType> cs;
  for (int i = 0; i < arg_num; ++i) {
    Variable v = ctx->get_var(vt, func_name);
    if (v.v_name == "") {
      return false;
    }
    vs.push_back(new VarExpr(v));
    if (i != 0) {
      int type = rand() % 4;
      cs.push_back(static_cast<CompType>(type + PLUS));
    }
  }
  Expr* prev = vs[0];
  for (int i = 0; i < cs.size(); ++i) {
    CompExpr* new_comp = new CompExpr(prev, vs[i+1], cs[i]);
    prev = new_comp;
  }
  Variable dst = ctx->get_var(vt, func_name);
  AssignExpr* ret = new AssignExpr(dst, prev);
  ctx->insert_func_norm_expr(func_name.c_str(), ret);
  return true;
}

bool decl_var(Context* ctx, VarType vt, string func_name, string vn) {
  ctx->def_func_var(func_name, vt);
}

bool gen_while(Context* ctx, string func_name) {
  
}