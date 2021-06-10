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
  return true;
}

string Context::code_gen() {
  return global_scope->code_gen();
}

bool Context::insert_func_arithmetic_comp(bool self_comp, CompType comp_type, VarType v_type, const char* func_name) {
  int num = self_comp? 2: 3;
  // find func scope
  Scope* func_scope = NULL;
  for (int i = 0; i < global_scope->exprs.size(); ++i) {
    if (global_scope->exprs[i]->type == FUNC && global_scope->exprs[i]->get_name() == func_name) {
      func_scope = global_scope->child_scopes.find(global_scope->exprs[i].get())->second;
      break;
    }
  }
  if (!func_scope) {
    return false;
  }
  // find variables
  vector<int> idxs;
  int lack = func_scope->get_vars(idxs, num, v_type);
  // define new variable
  for (int i = 0; i < lack; ++i) {
    func_scope->declare_var(INT);
  }
  if (lack > 0) {
    func_scope->get_vars(idxs, num, INT);
  }
  if (self_comp) {
    func_scope->gen_arithemetic_expr(idxs[0], idxs[1], -1, comp_type);
  } else {
    func_scope->gen_arithemetic_expr(idxs[0], idxs[1], idxs[2], comp_type);
  }
  return true;
}