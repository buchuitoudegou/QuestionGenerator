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
  main_scope->is_function = true;
  global_scope->exprs.push_back(main_decl);
  global_scope->scopes.push_back(main_scope);
  global_scope->child_scopes.insert(std::make_pair(main_decl.get(), main_scope.get()));
  return true;
}

bool Context::define_udf(const char* func_name, vector<VarType>& args, VarType ret_type) {
  shared_ptr<Expr> func_decl(new FuncExpr(func_name, ret_type, args));
  shared_ptr<Scope> func_scope(new Scope());
  func_scope->is_function = true;
  // add args to the current scope
  for (int i = 0; i < args.size(); ++i) {
    char temp = 'a' + func_scope->vars.size();
    string arg_name;
    arg_name.push_back(temp);
    func_scope->vars.push_back(Variable(arg_name.c_str(), args[i]));
  }
  global_scope->exprs.push_back(func_decl);
  global_scope->scopes.push_back(func_scope);
  global_scope->child_scopes.insert(std::make_pair(func_decl.get(), func_scope.get()));
  return true;
}

string Context::code_gen() {
  return global_scope->code_gen();
}

 Expr* Context::def_simple_comp(VarType v1, VarType v2, CompType ct, string func_name) {
  // find func scope
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    // not successful
    return NULL;
  }
  vector<int> var_idx;
  if (v1 == v2) {
    int lack = func_scope->get_vars(var_idx, 2, v1);
    if (lack > 0) {
      return NULL;
    }
  } else {
    vector<int> var_idx;
    int lack = func_scope->get_vars(var_idx, 1, v1);
    if (lack > 0) {
      return NULL;
    }
    lack = func_scope->get_vars(var_idx, 1, v2);
    if (lack > 0) {
      return NULL;
    }
  }
  CompExpr* ret = new CompExpr(
    new VarExpr(func_scope->vars[var_idx[0]]),
    new VarExpr(func_scope->vars[var_idx[1]]),
    ct
  );
  return ret;
}

bool Context::insert_func_ret_expr(const char* func_name) {
  Scope* func_scope = NULL;
  VarType ret_type = INT;
  func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return false;
  }
  if (ret_type == VOID) {
    // the function are supposed to return nothing
    return true;
  }
  // find variables
  vector<int> idxs;
  int lack = func_scope->get_vars(idxs, 1, ret_type);
  if (lack != 0) {
    return false;
  }
  func_scope->gen_ret_expr(idxs[0]);
  return true;
}

Variable Context::get_var(VarType v1, string func_name) {
  vector<int> idx;
  Scope* func_scope = get_func_scope(func_name);
  if (func_scope) {
    int lack = func_scope->get_vars(idx, 1, v1);
    if (lack > 0) {
      return Variable("", VOID);
    }
    return func_scope->vars[idx[0]];
  }
  return Variable("", VOID);
}

bool Context::insert_func_norm_expr(const char* func_name, Expr* e) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return false;
  }
  func_scope->insert_expr(e);
  return true;
}

Scope* Context::get_func_scope(string func_name) {
  Scope* func_scope = NULL;
  for (int i = 0; i < global_scope->exprs.size(); ++i) {
    if (global_scope->exprs[i]->type == FUNC && global_scope->exprs[i]->get_name() == func_name) {
      func_scope = global_scope->child_scopes.find(global_scope->exprs[i].get())->second;
      break;
    }
  }
  return func_scope;
}