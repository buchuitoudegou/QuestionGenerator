#include "../expression/expression.h"
#include <vector>
#include <fstream>

using std::ofstream;
using std::vector;

class Generator {
public:
  Generator();
  Generator(vector<Expr*> _exprs, string dst);
  void generate() const;
private:
  vector<Expr*> exprs;
  string dst;
};