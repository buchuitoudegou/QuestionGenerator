import ast
import random
import util
import ast_transform


def gen_distractor(ast_tree, config):
  if config["const"]:
    t = ast_transform.ConstTrans()
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
  target = None
  if isinstance(assign_stmt.value, ast.BinOp):
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
  rewrite = ast_transform.RewriteOp()
  rewrite.visit(stmt.test)
  # name_nodes = []
  # for node in ast.walk(stmt.test):
  #   if isinstance(node, ast.Name):
  #     name_nodes.append(node)
  # name_rewrite = ast_transform.RewriteName(name_nodes)
  # name_rewrite.visit(stmt.test)

def modify_range(stmt):
  iter_stmt = stmt.iter
  if isinstance(iter_stmt, ast.Call) and type(iter_stmt.func) == ast.Name and iter_stmt.func.id == 'range':
    for idx in range(len(iter_stmt.args)):
      arg = iter_stmt.args[idx]
      if isinstance(arg, ast.Constant):
        val = arg.value
        if isinstance(val, int):
          iter_stmt.args[idx] = ast.Constant(value=random.randint(-2, 2) + val, kind=None)
      if isinstance(arg, ast.Call):
        iter_stmt.args[idx] = ast.BinOp(
          left=arg,
          op=ast.Sub(),
          right=ast.Constant(value=random.randint(-2, 2), kind=None)
        )
  stmt.iter = iter_stmt