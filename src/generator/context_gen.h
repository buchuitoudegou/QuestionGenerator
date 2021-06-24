#ifndef __CONTEXT_GEN_H__
#define __CONTEXT_GEN_H__

#include "../context/context.h"
#include "../context/condition.h"
#include "../expression/assign_expr.h"
#include "../expression/for_expr.h"

Context* generate_context(uint16_t flags);

Expr* gen_arth(Context* ctx, VarType vt, string func_name);

Expr* gen_while(Context* ctx, string func_name, int arth_num);

Expr* gen_for(Context* ctx, string func_name, int arth_num);

bool decl_var(Context* ctx, VarType vt, string func_name);

#endif