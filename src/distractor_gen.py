import ast
from os import replace
import random
import util
import copy

def random_mod_str(string):
  s_arr = []
  for c in string:
    is_change = random.randint()
    if not is_change:
      s_arr.append(c)
      continue
    if c.isdigit():
      s_arr.append(str(random.randint(-1, 1) + int(c)))
    else:
      if c.isuppercase():
        s_arr.append(c.lower())
      else:
        s_arr.append(c.upper())
  return ''.join(s_arr)

class ConstTrans(ast.NodeTransformer):
  def visit_Constant(self, node):
    is_replace = random.randint(0, 1)
    if is_replace and isinstance(node.value, int):
      return ast.Constant(value=random.randint(-1, 1) + node.value, kind=None)
    if is_replace and isinstance(node.value, str):
      new_string = random_mod_str(node.value)
      return ast.Constant(value=new_string, kind=None)
    return node

class RewriteOp(ast.NodeTransformer):
  def replace_one(self, node):
    is_replace = random.randint(0, 1)
    replace_set = [ast.Eq, ast.Lt, ast.Gt, ast.GtE, ast.LtE, ast.NotEq]
    if is_replace:
      return util.rand_sel(replace_set)()
    else:
      return node
  
  def visit_Or(self, node):
    if random.randint(0, 1):
      return ast.And()
    else:
      return node
  def visit_And(self, node):
    if random.randint(0, 1):
      return ast.Or()
    else:
      return node
  def visit_Eq(self, node):
    return self.replace_one(node)
  def visit_Lt(self, node):
    return self.replace_one(node)
  def visit_Gt(self, node):
    return self.replace_one(node)
  def visit_LtE(self, node):
    return self.replace_one(node)
  def visit_GtE(self, node):
    return self.replace_one(node)
  def visit_NotEq(self, node):
    return self.replace_one(node)

def gen_distractor(ast_tree, config):
  if config["const"]:
    t = ConstTrans()
    t.visit(ast_tree)
  for stmt in ast_tree.body:
    if isinstance(stmt, ast.Assign) or isinstance(stmt, ast.AugAssign):
      if config["assignment"]:
        shuffle_assign(stmt)
    elif isinstance(stmt, ast.If) or isinstance(stmt, ast.While):
      if config["branch_sel"]:
        shuffle_test(stmt)
        gen_distractor(stmt, config)
    elif isinstance(stmt, ast.For):
      if config["for"]:
        modify_range(stmt)
        gen_distractor(stmt, config)
  return ast_tree

def shuffle_assign(assign_stmt):
  if not isinstance(assign_stmt.value, ast.BinOp):
    return
  target = assign_stmt.value
  while (isinstance(target, ast.BinOp)):
    # randomly replace the operator
    is_replace = random.randint(0, 1) == 1
    if is_replace and isinstance(target.right, ast.BinOp):
      temp = target.op
      target.op = target.right.op
      target.right.op = temp
    target = target.right
    
def shuffle_test(stmt):
  rewrite = RewriteOp()
  rewrite.visit(stmt.test)

def modify_range(stmt):
  iter_stmt = stmt.iter
  if isinstance(iter_stmt, ast.Call) and iter_stmt.func.id == 'range':
    for idx in range(len(iter_stmt.args)):
      arg = iter_stmt.args[idx]
      if isinstance(arg, ast.Constant):
        val = arg.value
        if isinstance(val, int):
          iter_stmt.args[idx] = ast.Constant(value=random.randint(0, 2) + val, kind=None)
      if isinstance(arg, ast.Call):
        iter_stmt.args[idx] = ast.BinOp(
          left=arg,
          op=ast.Sub(),
          right=ast.Constant(value=random.randint(1, 2), kind=None)
        )
  stmt.iter = iter_stmt