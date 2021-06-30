#include "context.h"

Scope::~Scope() {}

Scope::Scope() {}

bool Scope::declare_var(VarType v_type, string vn) {
  char _v = 'a' + vars.size();
  string v_name;
  v_name.push_back(_v);
  if (vn != "") {
    v_name = vn;
  }
  vars.push_back(Variable(v_name.c_str(), v_type));
  if (v_type == INT) {
    int init = rand() % 10;
    string init_val = std::to_string(init);
    shared_ptr<Expr> decl_expr(new DeclExpr(v_name.c_str(), v_type, init_val.c_str()));
    exprs.push_back(decl_expr);
  } else if (v_type == FLOAT) {
    int dec = rand() % 10;
    int fl = rand() % 20;
    string init_val = std::to_string(dec) + "." + std::to_string(fl);
    shared_ptr<Expr> decl_expr(new DeclExpr(v_name.c_str(), v_type, init_val.c_str()));
    exprs.push_back(decl_expr);
  }
  return true;
}

bool Scope::get_vars(vector<int>& ret, VarType v_type) {
  if (v_type == VOID) {
    return false;
  }
  for (int i = 0; i < vars.size(); ++i) {
    if (vars[i].v_type == v_type) {
      ret.push_back(i);
    }
  }
  return true;
}

bool Scope::insert_expr(Expr* e1) {
  shared_ptr<Expr> e(e1);
  exprs.push_back(e);
  return true;
}

string Scope::code_gen() {
  string code = "";
  for (int i = 0; i < exprs.size(); ++i) {
    code += exprs[i]->stringify();
    if (need_semicolon(exprs[i]->type)) {
      code += ";\n";
    }
    map<Expr*, Scope*>::iterator it = child_scopes.find(exprs[i].get());
    if (it != child_scopes.end()) {
      code += it->second->code_gen();
      code += "}\n";
    }
  }
  return code;
}

bool Scope::gen_ret_expr(int idx) {
  shared_ptr<Expr> ret_expr(new RetExpr(vars[idx].v_name.c_str()));
  exprs.push_back(ret_expr);
  return true;
}