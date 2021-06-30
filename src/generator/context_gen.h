#ifndef __CONTEXT_GEN_H__
#define __CONTEXT_GEN_H__

#include <map>

#include "../context/context.h"
#include "../context/condition.h"
#include "../expression/assign_expr.h"
#include "../expression/for_expr.h"
#include "../expression/while_expr.h"
#include "../expression/if_expr.h"

inline bool no_block(uint16_t flags) {
  return !((flags & IF) || (flags & WHILE) || (flags & FOR));
}

Context* generate_context(uint16_t flags, int var_num, int st_num, double app_ratio);

bool decl_var(Context* ctx, VarType vt, string func_name);

Expr* gen_arth(vector<Variable>& vars, bool is_compound);

WhileExpr* gen_while(vector<Variable>& vars, uint16_t flags);

ForExpr* gen_for(vector<Variable>& vars);

IfExpr* gen_if(vector<Variable>& vars, uint16_t flags);

ElseExpr* gen_else();

int select_from(bool filters[], int size);
// Expr* gen_if(Context* ctx, string func_name);

// bool insert_arth_to_scope(Context* ctx, string func_name, Expr* se, int arth_num);

#endif