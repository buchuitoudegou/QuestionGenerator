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
  bool declare_var(VarType v_type);
  int get_vars(vector<int>& ret, int num, VarType v_type);
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
  // declare and define main
  bool define_main();
  // define a udf
  bool define_udf(const char* func_name, vector<VarType>& args, VarType ret_type);
  // define simple computation between two vars
  Expr* def_simple_comp(VarType v1, VarType v2, CompType ct, string func_name);
  // get certain type of variable 
  Variable get_var(VarType v1, string func_name);
  // insert a function return expression that returns a random variable
  bool insert_func_ret_expr(const char* func_name);
  // insert a normal expression to function
  bool insert_func_norm_expr(const char* func_name, Expr* e);
  Scope* global_scope;
};

#endif