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

Expr* Context::insert_scope_expr(Expr* expr, string func_name, Expr* nst_expr) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return nullptr;
  }
  Scope* target_scope = nullptr;
  Scope* cur_scope = func_scope;
  if (nst_expr) {
    auto temp = cur_scope->child_scopes.find(nst_expr);
    if (temp != cur_scope->child_scopes.end()) {
      target_scope = temp->second;
    }
  } else {
    target_scope = func_scope;
  }
  if (!target_scope) {
    return nullptr;
  }
  shared_ptr<Expr> new_expr(expr);
  shared_ptr<Scope> new_scope(new Scope());
  new_scope->is_function = false;
  new_scope->vars = func_scope->vars;
  target_scope->exprs.push_back(new_expr);
  target_scope->scopes.push_back(new_scope);
  target_scope->child_scopes.insert(std::make_pair(new_expr.get(), new_scope.get()));
  return new_expr.get();
}

bool Context::insert_exprs_to_scope(Expr* sexpr, string func_name, vector<Expr*> exprs) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return false;
  }
  Scope* target_scope = nullptr;
  Scope* cur_scope = func_scope;
  while (true) {
    auto temp = cur_scope->child_scopes.find(sexpr);
    if (temp != cur_scope->child_scopes.end()) {
      target_scope = temp->second;
      break;
    } else {
      for (auto iter: cur_scope->child_scopes) {
        auto nst_temp = iter.second->child_scopes.find(sexpr);
        if (nst_temp != iter.second->child_scopes.end()) {
          target_scope = nst_temp->second;
        }
      }
      break;
    }
  }
  if (!target_scope) {
    return false;
  }
  // Scope* while_scope = func_scope->child_scopes.find(sexpr)->second;
  for (int i = 0; i < exprs.size(); ++i) {
    target_scope->insert_expr(exprs[i]);
  }
  return true;
}

string Context::code_gen() {
  return global_scope->code_gen();
}

bool Context::def_func_var(string func_name, VarType vt, string vn) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return false;
  }
  func_scope->declare_var(vt, vn);
  return true;
}

bool Context::insert_func_ret_expr(const char* func_name, RetExpr* ret_expr) {
  return insert_func_norm_expr(func_name, ret_expr);
}

vector<Variable> Context::get_vars(VarType v1, string func_name) {
  vector<int> idx;
  Scope* func_scope = get_func_scope(func_name);
  vector<Variable> ret;
  vector<int> idxs;
  if (!func_scope) {
    return {};
  }
  func_scope->get_vars(idxs, v1);
  for (auto idx: idxs) {
    ret.push_back(func_scope->vars[idx]);
  }
  return ret;
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