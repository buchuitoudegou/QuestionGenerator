#include "context.h"

Context::~Context() {
  if (global_scope)
    delete global_scope;
}

Context::Context() {
  global_scope = new Scope();
  global_scope->is_function = false;
}

bool Context::define_main() {
  shared_ptr<Expr> main_decl(new FuncExpr("main", INT, vector<VarType>()));
  // todo: args should be added into vars of the scope
  shared_ptr<Scope> main_scope = std::make_shared<Scope>();
  global_scope->scopes.push_back(main_scope);
  main_scope->is_function = true;
  global_scope->exprs.push_back(main_decl);
  global_scope->child_scopes.insert(std::make_pair(main_decl.get(), global_scope->scopes[0].get()));
}

string Context::code_gen() {
  return global_scope->code_gen();
}

bool Context::insert_main_arithmetic_comp(bool self_comp, CompType comp_type, VarType v_type) {
  int num = self_comp? 2: 3;
  // find variables
  vector<int> idxs;
  int lack = global_scope->scopes[0]->get_vars(idxs, num, v_type);
  for (int i = 0; i < lack; ++i) {
    global_scope->scopes[0]->declare_var(INT);
  }
  if (lack > 0) {
    global_scope->scopes[0]->get_vars(idxs, num, INT);
  }
  if (self_comp) {
    global_scope->scopes[0]->gen_arithemetic_expr(idxs[0], idxs[1], -1, comp_type);
  } else {
    global_scope->scopes[0]->gen_arithemetic_expr(idxs[0], idxs[1], idxs[2], comp_type);
  }
  return true;
}