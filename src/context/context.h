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
  bool gen_arithemetic_expr(int idx1, int idx2, int idx3, CompType comp_type);
};

class Context {
public:
  Context();
  ~Context();
  string code_gen();
  bool define_main();
  bool define_udf(const char* func_name, vector<VarType>& args, VarType ret_type);
  bool insert_func_arithmetic_comp(bool self_incr, CompType comp_type, VarType v_type, const char* func_name);
  // bool insert_func_ret_expr(const char* func_name);
  Scope* global_scope;
};

#endif