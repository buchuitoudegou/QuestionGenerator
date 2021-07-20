import ast
from random import randint
from typing import Generator
import expr
import astunparse
import materialize

IF       = 0x1   # if expression
WHILE    = 0x2   # while expression
FOR      = 0x4   # for expression
CMP_ASGN = 0x8   # compound calculation
CPLX_CND = 0x10  # complex condition (&& ||)
SLF_INC  = 0x20  # self increment/decrement
RDT_STM  = 0x40  # reduncdant statement
C_CND    = 0x80  # constant condition
VC_CND   = 0x100 # variable-constant condition
VV_CND   = 0x200 # variable-variable condition
VE_CND   = 0x400 # variable-expr condition
NST_BLK  = 0x800 # nested block

def gen_root():
  # return ast.parse('while a > 1:\n  a -= 1')
  return ast.Module(body=[], type_ignores=[])

def insert_expr_to_scope(expr, target):
  target.body.append(expr)

def gen_arth(vars, is_comp):
  arg_num = randint(2, 4)
  is_scomp = is_comp and (randint(0, 1) == 1)
  prev = None
  for _i in range(0, arg_num):
    is_const = randint(0, 1)
    rand_var = vars[randint(0, len(vars)-1)]
    temp = None
    if is_const:
      temp = ast.Constant(value=randint(1, 20), kind=None)
    else:
      temp = ast.Name(id=rand_var)
    if prev == None:
      prev = temp
    else:
      op = expr.conv_comp_to_ast(expr.rand_comp())
      prev = ast.BinOp(left=prev, op=op, right=temp)
  dst = vars[randint(0, len(vars)-1)]
  if is_scomp:
    op = expr.conv_comp_to_ast(expr.rand_comp())
    return ast.AugAssign(target=ast.Name(id=dst), op=op, value=prev)
  else:
    return ast.Assign(targets=[ast.Name(id=dst)], value=prev, type_comment=None)

def gen_while(vars, cond_type):
  expr = ast.While(test=None, body=[], orelse=[])
  comp = ast.Compare(left=None, ops=[], comparators=[])
  if randint(0, 1) == 1:
    comp.ops.append(ast.Gt())
  else:
    comp.ops.append(ast.GtE())
  if cond_type == VC_CND:
    comp.comparators.append(ast.Constant(value=randint(0, 20), kind=None))
    v_name = vars[randint(0, len(vars)-1)]
    comp.left = ast.Name(id=v_name)
  else:
    v1 = vars[randint(0, len(vars)-1)]
    v2 = vars[randint(0, len(vars)-1)]
    comp.comparators.append(ast.Name(id=v2))
    comp.left = ast.Name(id=v1)
  expr.test = comp
  expr.body.append(ast.AugAssign(target=ast.Name(id=v_name), op=ast.Sub(), value=ast.Constant(value=1, kind=None)))
  return expr

def gen_var(vars):
  v = ord('a') + len(vars)
  v = str(chr(v))
  vars.append(v)
  return ast.Assign(targets=[ast.Name(id=v)], value=ast.Constant(value=randint(-20, 20), kind=None), type_comment=None)

def gen_for():
  # declare a new temp var: _i
  for_stmt = ast.For(target=ast.Name(id='_i'), iter=ast.Call(func=ast.Name(id='range'), args=[
    ast.Constant(value=0, kind=None), ast.Constant(value=randint(0, 10), kind=None)
  ], keywords=[]), body=[], orelse=[])
  return for_stmt

def gen_ast(config):
  # with open('./template/t1.py') as f:
  #   tree = ast.parse(f.read())
  #   print()
  tree = gen_root()
  # expr = gen_while()
  vars = []
  var_expr = gen_var(vars)
  while_expr = gen_while(vars, VC_CND)
  insert_expr_to_scope(var_expr, tree)
  insert_expr_to_scope(while_expr, tree)
  arth_expr = gen_arth(vars, True)
  insert_expr_to_scope(arth_expr, while_expr)
  for_expr = gen_for()
  insert_expr_to_scope(for_expr, tree)
  arth_expr = gen_arth(vars + ['_i'], True)
  insert_expr_to_scope(arth_expr, for_expr)
  generator = materialize.CppGenerator('c++', tree)
  print(astunparse.dump(tree))
  print(astunparse.unparse(tree))
  print(generator.generate_code(vars))