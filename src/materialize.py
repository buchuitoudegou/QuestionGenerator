import ast
import expr

class Generator:
  def __init__(self, lang, ast_root):
    self.lang = lang
    self.tree = ast_root
  
  def generate_code(self):
    nodes = ast.iter_child_nodes(self.tree)
    code = ''
    for node in nodes:
      code += self.parse_stmt(node)
    return code

  def parse_assign(self, _):
    return ''
  
  def parse_augassign(self, _):
    return ''
  
  def parse_while(self, _):
    return ''
  
  def parse_for(self, _):
    return ''

  def parse_stmt(self, stmt):
    if isinstance(stmt, ast.Assign):
      return self.parse_assign(stmt)
    if isinstance(stmt, ast.While):
      return self.parse_while(stmt)
    if isinstance(stmt, ast.AugAssign):
      return self.parse_augassign(stmt)
    if isinstance(stmt, ast.For):
      return self.parse_for(stmt)
    return ''

  def parse_expr(self, _expr):
    return ''

class CppGenerator(Generator):
  def __init__(self, lang, ast_tree):
    Generator.__init__(self, lang, ast_tree)

  def generate_code(self, vars):
    code = f'#include <stdio.h>\nint main() {{\nint '
    for i in range(0, len(vars)):
      code += f'{vars[i]}'
      code += ',' if i != len(vars) - 1 else ';\n'
    code += Generator.generate_code(self)
    code += f'return 0;\n}}'
    return code

  def parse_assign(self, ast_stmt):
    dst = ast_stmt.targets[0].id
    left_expr = ast_stmt.value 
    return f'{dst} = {self.parse_expr(left_expr)};\n'
  
  def parse_while(self, ast_stmt):
    cond = ast_stmt.test
    children = ast.iter_child_nodes(ast_stmt)
    code = f'while ({self.parse_expr(cond)}) {{\n'
    for child in children:
      code += self.parse_stmt(child)
    code += '}\n'
    return code

  def parse_augassign(self, ast_stmt):
    dst = ast_stmt.target.id
    op = ast_stmt.op
    right = ast_stmt.value
    return f'{dst} {expr.stringify_ast(op)}= {self.parse_expr(right)};\n'

  def parse_for(self, ast_stmt):
    init = ast_stmt.iter.args[0].value
    end = ast_stmt.iter.args[1].value
    code = f'for (int _i = {init}; _i < {end}; ++i) {{\n'
    children = ast.iter_child_nodes(ast_stmt)
    for child in children:
      code += self.parse_stmt(child)
    code += '}\n'
    return code

  def parse_expr(self, ast_expr):
    if isinstance(ast_expr, ast.BinOp):
      op_str = expr.stringify_ast(ast_expr.op)
      return self.parse_expr(ast_expr.left) + op_str + self.parse_expr(ast_expr.right)
    if isinstance(ast_expr, ast.Constant):
      return str(ast_expr.value)
    if isinstance(ast_expr, ast.Name):
      return ast_expr.id
    if isinstance(ast_expr, ast.Compare):
      left = ast_expr.left
      op = ast_expr.ops[0]
      comparator = ast_expr.comparators[0]
      return self.parse_expr(left) + f' {expr.stringify_ast(op)} ' + self.parse_expr(comparator)
    return ''