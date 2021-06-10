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
  // vector<Scope> scopes;

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
  bool insert_main_arithmetic_comp(bool self_incr, CompType comp_type,  VarType v_type);
  Scope* global_scope;
};

#endif