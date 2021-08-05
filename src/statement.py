import ast
from random import randint
import util

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

def gen_arth(vars, var_types, arg_num, is_comp):
  is_scomp = is_comp and (randint(0, 1) == 1)
  const_vars = list(filter(lambda ele: var_types[ele] == 'Constant', vars))
  prev = None
  for _ in range(0, arg_num):
    is_const = randint(0, 1)
    rand_var = const_vars[randint(0, len(const_vars)-1)]
    temp = None
    if is_const:
      temp = ast.Constant(value=randint(1, 20), kind=None)
    else:
      temp = ast.Name(id=rand_var)
    if prev == None:
      prev = temp
    else:
      op = util.conv_comp_to_ast(util.rand_comp())
      prev = ast.BinOp(left=prev, op=op, right=temp)
  dst = vars[randint(0, len(vars)-1)]
  var_types[dst] = 'Constant'
  if is_scomp:
    op = util.conv_comp_to_ast(util.rand_comp())
    return ast.AugAssign(target=ast.Name(id=dst), op=op, value=prev)
  else:
    return ast.Assign(targets=[ast.Name(id=dst)], value=prev, type_comment=None)

def gen_while(vars, var_types, cond_type):
  expr = ast.While(test=None, body=[], orelse=[])
  comp = ast.Compare(left=None, ops=[], comparators=[])
  const_vars = list(filter(lambda ele: var_types[ele] == 'Constant', vars))
  if randint(0, 1) == 1:
    comp.ops.append(ast.Gt())
  else:
    comp.ops.append(ast.GtE())
  if cond_type == VC_CND:
    comp.comparators.append(ast.Constant(value=randint(0, 20), kind=None))
    v_name = const_vars[randint(0, len(const_vars)-1)]
    comp.left = ast.Name(id=v_name)
  else:
    v1 = const_vars[randint(0, len(const_vars)-1)]
    v2 = const_vars[randint(0, len(const_vars)-1)]
    comp.comparators.append(ast.Name(id=v2))
    comp.left = ast.Name(id=v1)
  expr.test = comp
  # ensure dead loop not exists
  expr.body.append(ast.AugAssign(target=ast.Name(id=v_name), op=ast.Sub(), value=ast.Constant(value=1, kind=None)))
  return expr

def gen_for():
  # declare a new temp var: _i
  for_stmt = ast.For(target=ast.Name(id='_i'), iter=ast.Call(func=ast.Name(id='range'), args=[
    ast.Constant(value=0, kind=None), ast.Constant(value=randint(0, 10), kind=None)
  ], keywords=[]), body=[], orelse=[])
  return for_stmt