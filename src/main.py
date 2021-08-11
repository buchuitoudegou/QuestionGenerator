import question_mod
import ast
from random import randint
import question_gen
import util
import generator
import argparse
import materialize
import distractor_gen
import json
import sys
import subprocess

def cmdparser():
  parser = argparse.ArgumentParser(description='AST Code Generator')
  parser.add_argument('--mode', type=str, default='conf')
  parser.add_argument('--path', type=str, default='question_config.json', help='path of the configuration')
  args = parser.parse_args()
  return args

def code_evaluate(tree):
  mat = materialize.PyGenerator("python", tree)
  code = mat.generate_code()
  with open('dst/temp.py', 'w+') as f:
    f.write(code)
    f.flush()
  try:
    popen = subprocess.Popen(["python3", "dst/temp.py"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, _ = popen.communicate(timeout=2)
    return out.decode(), code
  except:
    sys.stdout.flush()
    popen.kill()
    return '', code
  
  
def init_exprs():
  exprs = {
    "[STMT]": [],
    "[VAR_EXPR]": [],
    "[NUM_EXPR]": [],
    "[CONST_EXPR]": [],
    "[STR_EXPR]": [],
    "[BOOL_EXPR]": [],
    "[NUM_LIST_EXPR]": [],
    "[STR_LIST_EXPR]": [],
    "[BOOL_LIST_EXPR]": [],
    "[NUM_TUPLE_EXPR]": [],
    "[STR_TUPLE_EXPR]": [],
    "[STR_NUM_DICT_EXPR]": [],
    "[STR_STR_DICT_EXPR]": [],
    "[NUM_STR_DICT_EXPR]": [],
    "[LOOP_VAR]": [],
    "[SKIP_DEF]": [],
  }
  return exprs

def generate_var(exprs, 
                var_cnt, const_cnt, 
                num_cnt, str_cnt, 
                num_list_cnt, 
                str_list_cnt, 
                bool_cnt, num_str_dict_cnt,
                str_num_dict_cnt, str_str_dict_cnt,
                num_tuple_cnt=0, str_tuple_cnt=0):
  vars = []
  # generate native variables
  for i in range(var_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[VAR_EXPR]"].append(ast.Name(id=v))
  # generate constant
  for _ in range(const_cnt):
    v = randint(0, 20)
    exprs["[NUM_EXPR]"].append(ast.Constant(value=v, kind=None))
    exprs["[CONST_EXPR]"].append(ast.Constant(value=v, kind=None))
  # generate num var
  for _ in range(num_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[NUM_EXPR]"].append(ast.Name(id=v))
  # generate str var
  for _ in range(str_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[STR_EXPR]"].append(ast.Name(id=v))
  # generate num list var
  for _ in range(num_list_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[NUM_LIST_EXPR]"].append(ast.Name(id=v))
  # generate str list var
  for _ in range(str_list_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[STR_LIST_EXPR]"].append(ast.Name(id=v))
  # generate bool var
  for _ in range(bool_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[BOOL_EXPR]"].append(ast.Name(id=v))
  for _ in range(num_str_dict_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[NUM_STR_DICT_EXPR]"].append(ast.Name(id=v))
  for _ in range(str_str_dict_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[STR_STR_DICT_EXPR]"].append(ast.Name(id=v))
  for _ in range(str_num_dict_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[STR_NUM_DICT_EXPR]"].append(ast.Name(id=v))
  for _ in range(num_tuple_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[NUM_TUPLE_EXPR]"].append(ast.Name(id=v))
  for _ in range(str_tuple_cnt):
    v = ord('a') + len(vars)
    v = str(chr(v))
    vars.append(v)
    exprs["[STR_NUM_TUPLE_EXPR]"].append(ast.Name(id=v))
  

def gen_config(q_comps, stmt_cnt, var_config):
  tmp_ext = question_gen.TemplateFormulator()
  exprs = init_exprs()
  if "questions" in var_config:
    del var_config["questions"]
  if "distractor" in var_config:
    del var_config["distractor"]
  generate_var(exprs, **var_config)
  # generate config
  config = {
    "variables": {"amount": 0, "values":[]},
    "fixed_statements": {"amount": stmt_cnt, "stmt": []}
  }
  # statement definition
  for stmt_detail in q_comps:
    stmt = tmp_ext.formulate_stmt(stmt_detail, exprs)
    exprs["[STMT]"].append(stmt)
  # variable definition
  for e in exprs["[VAR_EXPR]"]:
    cur_type = util.rand_basic_type()
    # print(f"{e.id} in VAR")
    config["variables"]["values"].append(
      {"type": cur_type, "val": generator.gen_value(cur_type), "name": e.id}
    )
  for expr in exprs["[NUM_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      # print(f"{expr.id} in NUM")
      config["variables"]["values"].append(
        {"type": "Constant", "val": generator.gen_value("Constant"), "name": expr.id}
      )
  for expr in exprs["[STR_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      # print(f"{expr.id} in STR")
      config["variables"]["values"].append(
        {"type": "Str", "val": generator.gen_value("Str"), "name": expr.id}
      )

  for expr in exprs["[NUM_LIST_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      ele = "Constant"
      length = randint(0, 10)
      # print(f"{expr.id} in NUM_LIST")
      config["variables"]["values"].append(
        {"type": "List", "type_of_eles": [ele for _ in range(length)], "name": expr.id}
      )
  for expr in exprs["[STR_LIST_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      ele = "Str"
      length = randint(0, 10)
      # print(f"{expr.id} in NUM_LIST")
      config["variables"]["values"].append(
        {"type": "List", "type_of_eles": [ele for _ in range(length)], "name": expr.id}
      )
  for expr in exprs["[BOOL_LIST_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      ele = "Bool"
      length = randint(0, 10)
      config["variables"]["values"].append(
        {"type": "List", "type_of_eles": [ele for _ in range(length)], "name": expr.id}
      )
  for expr in exprs["[NUM_TUPLE_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      ele = "Constant"
      length = randint(0, 10)
      config["variables"]["values"].append(
        {"type": "Tuple", "type_of_eles": [ele for _ in range(length)], "name": expr.id}
      )
  for expr in exprs["[STR_TUPLE_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      ele = "Str"
      length = randint(0, 10)
      config["variables"]["values"].append(
        {"type": "Tuple", "type_of_eles": [ele for _ in range(length)], "name": expr.id}
      )
  for expr in exprs["[STR_NUM_DICT_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      key_ele = "Str"
      val_ele = "Constant"
      length = randint(0, 10)
      config["variables"]["values"].append(
        {
          "type": "Dict", 
          "type_of_keys": [key_ele for _ in range(length)],
          "type_of_vals": [val_ele for _ in range(length)],
          "name": expr.id
        }
      )
  for expr in exprs["[NUM_STR_DICT_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      key_ele = "Constant"
      val_ele = "Str"
      length = randint(0, 10)
      config["variables"]["values"].append(
        {
          "type": "Dict", 
          "type_of_keys": [key_ele for _ in range(length)],
          "type_of_vals": [val_ele for _ in range(length)],
          "name": expr.id
        }
      )
  for expr in exprs["[STR_STR_DICT_EXPR]"]:
    if isinstance(expr, ast.Name) and expr not in exprs["[SKIP_DEF]"]:
      key_ele = "Str"
      val_ele = "Str"
      length = randint(0, 10)
      config["variables"]["values"].append(
        {
          "type": "Dict", 
          "type_of_keys": [key_ele for _ in range(length)],
          "type_of_vals": [val_ele for _ in range(length)],
          "name": expr.id
        }
      )
  for expr in exprs["[BOOL_EXPR]"]:
    ele = "Bool"
    config["variables"]["values"].append(
        {"type": "Bool", "val": generator.gen_value("Bool"), "name": expr.id}
      )
  for stmt in exprs["[STMT]"]:
    config["fixed_statements"]["stmt"].append(stmt)
  return config

def gen_conf_tree(config):
  q_conf = config["questions"]
  options, tree = [], None
  while True:
    code_config = gen_config(q_conf, len(q_conf), config)
    tree = generator.gen_fixed_ast(code_config)
    ret, code = code_evaluate(tree)
    if ret != '':
      options.append(ret)
      with open('dst/target.py', 'w') as f:
        f.write(code)
        f.flush()
      break
  return tree, options

def gen_mod_tree(config):
  mod = question_mod.QuestionModifier(config["code_path"], config["distractor"])
  options, tree = [], None
  while True:
    tree = mod.gen_new_tree()
    ret, code = code_evaluate(tree)
    if ret != '':
      options.append(ret)
      with open('dst/target.py', 'w') as f:
        f.write(code)
        f.flush()
      break
  return tree, options


if __name__ == "__main__":
  cmd = cmdparser()
  config_path, mode = cmd.path, cmd.mode
  config = json.load(open(config_path))
  dis_config = config["distractor"]
  options, tree = None, None
  if mode == 'conf':
    tree, options = gen_conf_tree(config)
  if mode == 'mod':
    tree, options = gen_mod_tree(config)
  print(options)
  # generate distractor
  while len(options) < dis_config["cnt"]:
    distractor_gen.gen_distractor(tree, dis_config)
    ret, _ = code_evaluate(tree)
    if ret != '' and ret not in options:
      options.append(ret)
      print(options)