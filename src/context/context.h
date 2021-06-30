#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <stdlib.h>
#include "../expression/expression.h"
#include "../expression/func_expr.h"
#include "../expression/decl_expr.h"
#include "../expression/variable.h"
#include "../expression/comp_expr.h"
#include "../expression/while_expr.h"

using std::vector;
using std::string;
using std::map;
using std::shared_ptr;
using std::make_shared;

class Scope {
public:
  vector<Variable> vars;
  vector<shared_ptr<Expr> > exprs;
  vector<shared_ptr<Scope> > scopes;
  bool is_function;
  map<Expr*, Scope*> child_scopes;
  Scope();
  ~Scope();
  string code_gen();
  bool declare_var(VarType v_type, string v_name="");
  bool get_vars(vector<int>& ret, VarType v_type);
  bool gen_ret_expr(int idx);
  bool insert_expr(Expr* expr);
};

class Context {
private:
  Scope* get_func_scope(string func_name);
public:
  Context();
  ~Context();
  string code_gen();
  // get certain type of variable 
  vector<Variable> get_vars(VarType v1, string func_name);
  // declare and define main
  bool define_main();
  // define a udf
  bool define_udf(const char* func_name, vector<VarType>& args, VarType ret_type);
  // define new variable in function
  bool def_func_var(string func_name, VarType vt, string vn="");
  // insert a function return expression that returns a variable
  bool insert_func_ret_expr(const char* func_name, RetExpr* ret_expr);
  // insert a normal expression to function
  bool insert_func_norm_expr(const char* func_name, Expr* e);
  // insert a while block to function
  Expr* insert_scope_expr(Expr* expr, string func_name, Expr* nst_expr=nullptr);
  // insert expressions to a while block
  bool insert_exprs_to_scope(Expr* we, string func_name, vector<Expr*> exprs);
  Scope* global_scope;
};

#endif