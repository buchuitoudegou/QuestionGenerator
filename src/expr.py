import ast
from random import randint

comp_type = ['PLUS', 'SUB', 'TIMES', 'DIV', 'MOD']
compare_type = ['EQUAL', 'LARGER', 'LESS', 'NO_LESS', 'NO_LARGER']
conn_type = ['OR', 'AND']

def rand_sel(arr):
  return arr[randint(0, len(arr)-1)]

def rand_comp():
  return rand_sel(comp_type)

def rand_cmp():
  return rand_sel(compare_type)

def conv_comp_to_ast(ct):
  if ct == 'PLUS':
    return ast.Add()
  if ct == 'SUB':
    return ast.Sub()
  if ct == 'TIMES':
    return ast.Mult()
  if ct == 'DIV':
    return ast.Div()
  if ct == 'MOD':
    return ast.Mod()

def conv_cmp_to_ast(ct):
  if ct == 'EQUAL':
    return ast.Eq()
  if ct == 'LARGER':
    return ast.Gt()
  if ct == 'LESS':
    return ast.Lt()
  if ct == 'NO_LESS':
    return ast.LtE()
  if ct == 'NO_LARGER':
    return ast.GtE()
  if ct == 'AND':
    return ast.And()
  if ct == 'OR':
    return ast.Or()

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