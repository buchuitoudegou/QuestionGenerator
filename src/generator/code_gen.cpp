#include "code_gen.h"

string code_generation(Context* context) {
  // todo: generate different code type regarding the input condition
  // modify the generated code from context
  return "#include <stdio.h>\n" + context->code_gen();
}