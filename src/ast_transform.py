import ast
import random
import util
import string

def random_mod_str(strs):
  s_arr = []
  for c in strs:
    is_change = random.randint(0, 1)
    if not is_change:
      s_arr.append(c)
      continue
    if c.isdigit():
      s_arr.append(str(random.randint(-1, 1) + int(c)))
    else:
      if random.randint(0, 10) == 1:
        chars = string.ascii_lowercase + string.ascii_uppercase
        s_arr.append(random.choice(chars))
      elif c.isupper():
        s_arr.append(c.lower())
      else:
        s_arr.append(c.upper())
  return ''.join(s_arr)

class ConstTrans(ast.NodeTransformer):
  def visit_List(self, node: ast.List):
    # shuffle and modify the length
    is_replace = random.randint(0, 1)
    if not is_replace:
      return node
    list_content = node.elts
    random.shuffle(list_content)
    if len(list_content) > 0 and random.randint(0, 10) == 1:
      rand_len = random.randint(1, len(list_content))
      list_content = random.sample(list_content, rand_len)
    node.elts = list_content
    return node
  def visit_Tuple(self, node: ast.Tuple):
    is_replace = random.randint(0, 1)
    if not is_replace:
      return node
    return self.visit_List(node)

  def visit_Constant(self, node):
    is_replace = random.randint(0, 1)
    if is_replace and type(node.value) ==  int:
      return ast.Constant(value=random.randint(-1, 1) + node.value, kind=None)
    if is_replace and type(node.value) ==  str:
      new_string = random_mod_str(node.value)
      return ast.Constant(value=new_string, kind=None)
    if is_replace and type(node.value) ==  bool:
      return ast.Constant(value=random.randint(0, 1) == 1, kind=None)
    return node

class RewriteOp(ast.NodeTransformer):
  def __init__(self):
    ast.NodeTransformer.__init__(self)
    self.replace_set = [ast.Eq, ast.Lt, ast.Gt, ast.GtE, ast.LtE, ast.NotEq]
  def replace_one(self, node):
    is_replace = random.randint(0, 1)
    if is_replace:
      return util.rand_sel(self.replace_set)()
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

# class RewriteName(ast.NodeTransformer):
#   def __init__(self, replace_list):
#     ast.NodeTransformer.__init__(self)
#     self.replace_list = replace_list
  
#   def visit_Name(self, node):
#     is_replace = random.randint(0, 1)
#     if is_replace:
#       return util.rand_sel(self.replace_list)
#     else:
#       return node

class AssignmentRewrite(ast.NodeTransformer):
  def visit_Assign(self, node: ast.Assign):
    # shuffling the operator
    target = None
    if isinstance(node.value, ast.BinOp):
      target = node.value
    else:
      return node
    while (type(target) == ast.BinOp):
      is_replace = random.randint(0, 1)
      if is_replace and type(target.right, ast.BinOp):
        temp = target.op
        target.op = target.right.op
        target.right.op = temp
      target = target.right
    return target

  def visit_AugAssign(self, node):
    return self.visit_Assign(node)

class RerangeFor(ast.NodeTransformer):
  def visit_For(self, node: ast.For):
    iter_stmt = node.iter
    if type(iter_stmt) == ast.Call:
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
    node.iter = iter_stmt
    return node

class BranchRewrite(ast.NodeTransformer):
  def visit_If(self, node: ast.If):
    rewrite = RewriteOp()
    rewrite.visit(node.test)
    return node

  def visit_While(self, node: ast.While):
    return self.visit_If(node)