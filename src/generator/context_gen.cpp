#include "context_gen.h"

Context* generate_context(uint8_t flags) {
  Context* context = new Context();
  // todo: generate context that fulfill the condition flag
  context->define_main();
  context->insert_func_arithmetic_comp(true, PLUS, INT, "main");
  return context;
}