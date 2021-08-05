import ast
from random import randint
import random
import string
import util
import statement
import astunparse

def gen_root():
  return ast.Module(body=[], type_ignores=[])

def gen_string(size=6, chars=string.ascii_lowercase + string.digits + string.ascii_uppercase):
  return ''.join(random.choice(chars) for _ in range(size))

def gen_value(type):
  if type == "Constant":
    value = randint(-20, 20)
  elif type == "Str":
    value = gen_string()
  elif type == "Bool":
    value = False if randint(0, 1) else True
  return value

def gen_payload(var_detail):
  if var_detail["type"] == "Constant" or var_detail["type"] == "Str" or var_detail["type"] == "Bool":
    payload = {"value": var_detail["val"], "kind": None}
  if var_detail["type"] == "List":
    payload = {"elts": []}
    for ele_type in var_detail["type_of_eles"]:
      payload["elts"].append(util.conv_var_to_ast("Constant")(value=gen_value(ele_type), kind=None))
  if var_detail["type"] == "Dict":
    payload = {"keys": [], "values": []}
    for ele_type in var_detail["type_of_keys"]:
      payload["keys"].append(util.conv_var_to_ast("Constant")(value=gen_value(ele_type), kind=None))
    for ele_type in var_detail["type_of_vals"]:
      payload["values"].append(util.conv_var_to_ast("Constant")(value=gen_value(ele_type), kind=None))
  return payload

def gen_var(vars, var_types, var_detail, name=''):
  if name == '':
    v = ord('a') + len(vars)
    v = str(chr(v))
  else:
    v = name
  vars.append(v)
  var_types[v] = var_detail["type"]
  is_rand = var_detail["type"] == "Constant" and var_detail["val"] == "rand"
  if is_rand:
    return ast.Assign(targets=[ast.Name(id=v)], value=ast.Constant(value=randint(-20, 20), kind=None), type_comment=None)
  else:
    val = util.conv_var_to_ast(var_detail["type"])
    payload = gen_payload(var_detail)
    return ast.Assign(targets=[ast.Name(id=v)], value=val(**payload))

def gen_statement(stmt_detail, vars, var_types):
  stmt = None
  if stmt_detail["type"] == "Arth":
    stmt = statement.gen_arth(vars, var_types, stmt_detail["arg_num"], stmt_detail["is_self_comp"])
  elif stmt_detail["type"] == "While":
    stmt = statement.gen_while(vars, var_types, stmt_detail["cond_type"])
    for child in stmt_detail["body"]:
      child_stmt = gen_statement(child, vars, var_types)
      stmt.body.append(child_stmt)
  return stmt

def gen_random_ast(config):
  tree = gen_root()
  vars = []
  var_types = {}
  # generate variables
  for var_detail in config["variables"]["values"]:
    var_stmt = gen_var(vars, var_types, var_detail)
    tree.body.append(var_stmt)
  # generate statments
  for stmt_detail in config["statements"]["stmt"]:
    stmt = gen_statement(stmt_detail, vars, var_types)
    tree.body.append(stmt)
  print(astunparse.unparse(tree))

def gen_fixed_ast(config):
  tree = gen_root()
  vars = []
  var_types = {}
  # generate variables
  for var_detail in config["variables"]["values"]:
    var_stmt = gen_var(vars, var_types, var_detail, var_detail["name"])
    tree.body.append(var_stmt)
  # insert statement
  for stmt in config["fixed_statements"]["stmt"]:
    tree.body.append(stmt)
  return tree