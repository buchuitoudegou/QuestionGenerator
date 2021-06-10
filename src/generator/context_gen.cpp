#include "context_gen.h"

Context* generate_context(uint8_t flags) {
  Context* context = new Context();
  // todo: generate context that fulfill the condition flag
  context->define_main();
  context->insert_main_arithmetic_comp(true, PLUS, INT);
  return context;
}