import ast
from random import randint
import re

comp_type = ['PLUS', 'SUB', 'TIMES', 'DIV', 'MOD']
compare_type = ['EQUAL', 'LARGER', 'LESS', 'NO_LESS', 'NO_LARGER']
conn_type = ['OR', 'AND']
basic_type = ['Constant', 'Str', 'Bool']

priority = {
  '+': 5,
  '-': 5,
  '*': 6,
  '/': 6,
  '%': 6,
  '[]': 10,
  '>': 4,
  '<': 4,
  '>=': 4,
  '<=': 4,
  '==': 4,
  '!=': 4,
  'and': 3,
  'or': 3
}

def format_op(exprs, type, op=None):
  if type == 'BinOp':
    return ast.BinOp(left=exprs[0], op=op(), right=exprs[1])
  if type == 'Subscript':
    return ast.Subscript(value=exprs[0], slice=ast.Index(value=exprs[1]))
  if type == 'Compare':
    return ast.Compare(left=exprs[0], ops=[op()], comparators=[exprs[1]])
  if type == 'BoolOp':
    values = [exprs[i] for i in exprs.keys()]
    return ast.BoolOp(op=op(), values=values)
  if type == 'Call':
    args = list(filter(lambda x: x != 0, exprs.keys()))
    args = [exprs[i] for i in args]
    return ast.Call(func=ast.Attribute(value=exprs[0], attr=op), args=args, keywords=[])

def priority_no_less(a, b):
  if isinstance(a(), ast.Subscript):
    return True
  if isinstance(b, str):
    return False
  return priority[stringify_ast(a())] >= priority[stringify_ast(b())]

def rand_sel(arr):
  return arr[randint(0, len(arr)-1)]

def rand_comp():
  return rand_sel(comp_type)

def rand_cmp():
  return rand_sel(compare_type)

def rand_basic_type():
  return rand_sel(basic_type)

def conv_comp_to_ast(ct):
  if ct == 'PLUS':
    return ast.Add
  if ct == 'SUB':
    return ast.Sub
  if ct == 'TIMES':
    return ast.Mult
  if ct == 'DIV':
    return ast.Div
  if ct == 'MOD':
    return ast.Mod
  if ct == 'IDX':
    return ast.Subscript
  if ct == 'LARGER':
    return ast.Gt
  if ct == 'EQUAL':
    return ast.Eq
  if ct == 'LARGER':
    return ast.Gt
  if ct == 'LESS':
    return ast.Lt
  if ct == 'NO_LESS':
    return ast.LtE
  if ct == 'NO_LARGER':
    return ast.GtE
  if ct == 'AND':
    return ast.And
  if ct == 'OR':
    return ast.Or
  if ct == 'AUGASSIGN_ADD':
    return ast.Add
  if ct == 'STR_UPPERCASE':
    return 'upper'
  if ct == 'LIST_APPEND':
    return 'append'
  if ct == 'FUNC_CALL':
    return 'func_call'

def stringify_ast(ast_op):
  if isinstance(ast_op, ast.Add):
    return '+'
  if isinstance(ast_op, ast.Sub):
    return '-'
  if isinstance(ast_op, ast.Mult):
    return '*'
  if isinstance(ast_op, ast.Div):
    return '/'
  if isinstance(ast_op, ast.Mod):
    return '%'
  if isinstance(ast_op, ast.Eq):
    return '=='
  if isinstance(ast_op, ast.Gt):
    return '>'
  if isinstance(ast_op, ast.Lt):
    return '<'
  if isinstance(ast_op, ast.LtE):
    return '<='
  if isinstance(ast_op, ast.GtE):
    return '>='
  if isinstance(ast_op, ast.Subscript):
    return '[]'
  if isinstance(ast_op, ast.Or):
    return 'or'
  if isinstance(ast_op, ast.And):
    return 'and'

def conv_var_to_ast(vt):
  if vt == 'Dict':
    return ast.Dict
  if vt == 'Constant' or vt == 'Str' or vt == 'Bool':
    return ast.Constant
  if vt == 'List':
    return ast.List

def str_to_stmt(st):
  if st == 'ASSIGN':
    return ast.Assign
  if re.match(r"AUGASSIGN_*", st):
    return ast.AugAssign