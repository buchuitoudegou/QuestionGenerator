import ast
from random import randint
import question_gen
import util
import generator
import argparse
import materialize
import distractor_gen
import json
import subprocess

dis_config = {
  "const": True,
  "assignment": True,
  "branch_sel": True,
  "for": True
}

def cmdparser():
  parser = argparse.ArgumentParser(description='AST Code Generator')
  parser.add_argument('--path', type=str, default='question_config.json', help='path of the configuration')
  args = parser.parse_args()
  return args

def code_evaluate(tree, in_file):
  print(f"start evaluating: {in_file}")
  mat = materialize.PyGenerator("python", tree)
  code = mat.generate_code()
  with open(in_file, 'w') as f:
    f.write(code)
    f.flush()
  
  
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
    "[RND_LIST_EXPR]": [],
    "[LOOP_VAR]": [],
    "[SKIP_DEF]": []
  }
  return exprs

def generate_var(exprs, var_cnt, const_cnt, num_cnt, str_cnt, num_list_cnt, str_list_cnt, bool_cnt):
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
  

def gen_config(q_comps, stmt_cnt, var_config):
  tmp_ext = question_gen.TemplateFormulator()
  exprs = init_exprs()
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
  # print(exprs)
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
  for expr in exprs["[BOOL_EXPR]"]:
    ele = "Bool"
    config["variables"]["values"].append(
        {"type": "Bool", "val": generator.gen_value("Bool"), "name": expr.id}
      )
  for stmt in exprs["[STMT]"]:
    config["fixed_statements"]["stmt"].append(stmt)
  return config

if __name__ == "__main__":
  config_path = cmdparser().path
  config = json.load(open(config_path))
  q_conf = config["question"]
  del config["question"]
  gen_config = gen_config(q_conf, len(q_conf), config)
  tree = generator.gen_fixed_ast(gen_config)
  ret = code_evaluate(tree, "dst/target.py")
  # generate distractor
  cnt = 30
  idx = 0
  while cnt > 0:
    distractor_gen.gen_distractor(tree, dis_config)
    ret = code_evaluate(tree, f"dst/target-{idx}.py")
    idx += 1
    cnt -= 1