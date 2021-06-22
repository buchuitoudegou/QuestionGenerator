#ifndef __CONTEXT_GEN_H__
#define __CONTEXT_GEN_H__

#include "../context/context.h"
#include "../context/condition.h"
#include "../expression/assign_expr.h"

Context* generate_context(uint16_t flags);

#endif