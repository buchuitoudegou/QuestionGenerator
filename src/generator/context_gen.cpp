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
      Expr* ret = gen_arth(context, INT, "foo");
      context->insert_func_norm_expr("foo", ret);
    }
    context->insert_func_ret_expr("foo");
  }
  context->define_main();
  if (flags & ARTH) {
    decl_var(context, INT, "main");
    decl_var(context, INT, "main");
    Expr* ret = gen_arth(context, INT, "main");
    context->insert_func_norm_expr("main", ret);
  }
  if (flags & WHILE_LOOP) {
    gen_while(context, "main", 3);
  }
  if (flags & FOR_LOOP) {
    gen_for(context, "main", 4);
  }
  return context;
}

Expr* gen_arth(Context* ctx, VarType vt, string func_name)  {
  int arg_num = rand() % 2 + 2;
  vector<VarExpr*> vs;
  vector<CompType> cs;
  for (int i = 0; i < arg_num; ++i) {
    Variable v = ctx->get_var(vt, func_name);
    if (v.v_name == "") {
      return nullptr;
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
  // ctx->insert_func_norm_expr(func_name.c_str(), ret);
  return ret;
}

bool decl_var(Context* ctx, VarType vt, string func_name) {
  ctx->def_func_var(func_name, vt);
}

Expr* gen_while(Context* ctx, string func_name, int arth_num) {
  Variable v = ctx->get_var(INT, func_name);
  Expr* ce = new ConstExpr("4");
  Expr* ctrl_expr = new BoolExpr(v, ce, LARGER);
  Expr* we = ctx->insert_while(ctrl_expr, func_name);
  vector<Expr*> arths;
  for (int i = 0; i < arth_num; ++i) {
    // int type = rand() % 2;
    Expr* temp = gen_arth(ctx, INT, func_name);
    arths.push_back(temp);
  }
  ctx->insert_while_exprs(we, func_name, arths);
  return we;
}

Expr* gen_for(Context* ctx, string func_name, int arth_num) {
  Expr* init = new DeclExpr("i", INT, "0");
  Expr* ctrl = new BoolExpr(Variable("i", INT), new ConstExpr("10"), LESS);
  Expr* iter = new SelfCompExpr(Variable("i", INT), PLUS, "1");
  Expr* for_expr = new ForExpr(init, ctrl, iter);
  Expr* fe = ctx->insert_for(for_expr, func_name);
  vector<Expr*> arths;
  for (int i = 0; i < arth_num; ++i) {
    // int type = rand() % 2;
    Expr* temp = gen_arth(ctx, INT, func_name);
    arths.push_back(temp);
  }
  ctx->insert_while_exprs(fe, func_name, arths);
  return fe;
}