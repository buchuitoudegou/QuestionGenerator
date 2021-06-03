#include "generator.h"

Generator::Generator() {
  exprs = vector<Expr*>();
  dst = "";
}

Generator::Generator(vector<Expr*> _e, string d):
  exprs(_e), dst(d) {}

void Generator::generate() const {
  string code = "#include <iostream>\nusing namespace std;\nint main() {\n";
  for (int i = 0; i < exprs.size(); ++i) {
    code += exprs[i]->stringify();
  }
  code += "return 0;\n}\n";
  ofstream output;
  output.open(dst);
  output << code;
  output.close();
}