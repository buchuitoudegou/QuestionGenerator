#ifndef __CONTEXT_GEN_H__
#define __CONTEXT_GEN_H__

#include "../context/context.h"
#include "../context/condition.h"
#include "../expression/assign_expr.h"

Context* generate_context(uint16_t flags);

bool gen_arth(Context* ctx, VarType vt, string func_name);

bool decl_var(Context* ctx, VarType vt, string func_name, string vn="");

bool gen_while(Context* ctx, string func_name, int arth_num);

#endif