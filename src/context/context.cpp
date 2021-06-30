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

Expr* Context::insert_while(Expr* ctrl_expr, string func_name) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return NULL;
  }
  shared_ptr<Expr> while_expr(new WhileExpr(ctrl_expr));
  shared_ptr<Scope> while_scope(new Scope());
  while_scope->is_function = false;
  while_scope->vars = func_scope->vars;
  func_scope->exprs.push_back(while_expr);
  func_scope->scopes.push_back(while_scope);
  func_scope->child_scopes.insert(std::make_pair(while_expr.get(), while_scope.get()));
  return while_expr.get();
}

Expr* Context::insert_for(Expr* for_expr, string func_name) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return NULL;
  }
  shared_ptr<Expr> fe(for_expr);
  shared_ptr<Scope> for_scope(new Scope());
  for_scope->is_function = false;
  for_scope->vars = func_scope->vars;
  func_scope->exprs.push_back(fe);
  func_scope->scopes.push_back(for_scope);
  func_scope->child_scopes.insert(std::make_pair(fe.get(), for_scope.get()));
  return fe.get();
}


Expr* Context::insert_if(Expr* _ife, string func_name) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return nullptr;
  }
  shared_ptr<Expr> ife(_ife);
  shared_ptr<Scope> if_scope(new Scope());
  if_scope->is_function = false;
  if_scope->vars = func_scope->vars;
  func_scope->exprs.push_back(ife);
  func_scope->scopes.push_back(if_scope);
  func_scope->child_scopes.insert(std::make_pair(ife.get(), if_scope.get()));
  return ife.get();
}

bool Context::insert_while_exprs(Expr* we, string func_name, vector<Expr*> exprs) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return false;
  }
  Scope* while_scope = func_scope->child_scopes.find(we)->second;
  for (int i = 0; i < exprs.size(); ++i) {
    while_scope->insert_expr(exprs[i]);
  }
  return true;
}

bool Context::insert_for_exprs(Expr* fe, string func_name, vector<Expr*> exprs) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return false;
  }
  Scope* for_scope = func_scope->child_scopes.find(fe)->second;
  for (int i = 0; i < exprs.size(); ++i) {
    for_scope->insert_expr(exprs[i]);
  }
  return true;
}

bool Context::insert_if_exprs(Expr* ife, string func_name, vector<Expr*> exprs) {
  Scope* func_scope = get_func_scope(func_name);
  if (!func_scope) {
    return false;
  }
  Scope* for_scope = func_scope->child_scopes.find(ife)->second;
  for (int i = 0; i < exprs.size(); ++i) {
    for_scope->insert_expr(exprs[i]);
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

bool Context::insert_func_ret_expr(const char* func_name, const char* v_name) {
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
  func_scope->get_vars(idxs, ret_type);
  if (idxs.size() > 0) {
    int target = -1;
    for (auto idx: idxs) {
      if (func_scope->vars[idx].v_name == v_name) {
        target = idx;
        break;
      }
    }
    if (target != -1) {
      func_scope->gen_ret_expr(target);
      return true;
    } else {
      return false;
    }
  }
  // no variable
  return false;
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