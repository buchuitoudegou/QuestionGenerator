import ast
from random import randint
import json
import util
import re

def expr_lookup(exprs, target, no_constant=False):
  if re.match(r"\[.*NOCST\]", target):
    no_constant = True
    target = target.replace('_NOCST', '')
  candidates = []
  if target == "[VAR_EXPR]":
    for key in exprs.keys():
      if key != "[STMT]":
        candidates += exprs[key]
  else:
    for key in exprs.keys():
      if key == target:
        candidates += exprs[key]
      elif target != "[LOOP_VAR]" and key == "[VAR_EXPR]":
        candidates += exprs[key]
  if no_constant:
    candidates = list(filter(lambda item: not isinstance(item, ast.Constant), candidates))
  ridx = 0
  if len(candidates) > 1:
    ridx = randint(0, len(candidates) - 1)
  if len(candidates) == 0:
    print(target, "error!!!!!\n\n\n")
  ret = candidates[ridx]
  if ret in exprs["[VAR_EXPR]"]:
    exprs["[VAR_EXPR]"].remove(ret)
  if ret not in exprs[target]:
    exprs[target].append(ret)
  return ret

class TemplateFormulator():
  def __init__(self):
    self.templates = json.load(open('template.json'))

  def get(self, tname):
    return self.templates[tname]
  
  def get_op_list(self, right_ops):
    right_ops = [self.get(right_ops[i]) for i in range(len(right_ops))]
    comb_list = [rop["ast"] for rop in right_ops]
    op_str_list = [rop["op"] for rop in right_ops]
    op_ast_list = [util.conv_comp_to_ast(op_str) for op_str in op_str_list]
    op_ret_list = [rop["return"] for rop in right_ops]
    op_templates = [rop["template"] for rop in right_ops]
    ret = []
    for i in range(len(op_ast_list)):
      tmp = {
        "ast_op": comb_list[i],
        "op": op_ast_list[i],
        "exprs": {},
        "return": op_ret_list[i],
        "template": op_templates[i]
      }
      ret.append(tmp)
    return ret

  def combine_expr(self, op_list, exprs):
    while len(op_list) > 0:
      cur = op_list[0]
      cur_idx = 0
      for i in range(1, len(op_list)):
        if not util.priority_no_less(cur["op"], op_list[i]["op"]):
          cur = op_list[i]
          cur_idx = i
      for i in range(len(cur["template"])):
        if i not in cur["exprs"]:
          cur["exprs"][i] = expr_lookup(exprs, cur["template"][i])
      new_expr = util.format_op(cur["exprs"], cur["ast_op"], cur["op"])
      if len(op_list) == 1:
        return new_expr
      if cur_idx == 0 and len(op_list) > 1:
        # [cur_op, next_op]
        next = op_list[cur_idx + 1]
        next["exprs"][0] = new_expr
      elif cur_idx > 0 and cur_idx == len(op_list) - 1:
        # [prev_op, cur_op]
        prev = op_list[cur_idx - 1]
        prev["exprs"][len(prev["template"]) - 1] = new_expr
      else:
        # [prev_op, cur_op, next_op]
        prev = op_list[cur_idx - 1]
        next = op_list[cur_idx + 1]
        if not util.priority_no_less(prev["op"], next["op"]):
          prev["exprs"][len(prev["template"]) - 1] = new_expr
        else:
          next["exprs"][0] = new_expr
      op_list.remove(cur)

  def formulate_assign(self, stmt_details, exprs):
    stmt_type = stmt_details["st"]
    right_ops = stmt_details["ops"]
    final_expr, dst = None, None
    if len(right_ops) > 0:
      right_op_list = self.get_op_list(right_ops)
      last_ret = self.get(right_ops[-1])["return"]
      if "dst" in stmt_details:
        dst = expr_lookup(exprs, stmt_details["dst"], True)
      else:
        dst = expr_lookup(exprs, last_ret, True)
      final_expr = self.combine_expr(right_op_list, exprs)
    else:
      dst = expr_lookup(exprs, stmt_details["dst"], True)
      final_expr = expr_lookup(exprs, stmt_details["dst"])
    if stmt_type == 'ASSIGN':
      return util.str_to_stmt(stmt_type)(targets=[dst], value=final_expr, type_comment=None)
    else:
      return util.str_to_stmt(stmt_type)(target=dst, op=util.conv_comp_to_ast(stmt_type)(), value=final_expr)

  def formulate_if_while(self, stmt_details, exprs):
    stmt_type = stmt_details["st"]
    test_ops = stmt_details["test"]
    test_expr = None
    if test_ops[0] == 'BOOL_CONST':
      val = True if randint(0, 1) else False
      test_expr = ast.Constant(value=val, kind=None)
    elif test_ops[0] == 'NUM_CONST':
      val = randint(0, 1)
      test_expr = ast.Constant(value=val, kind=None)
    else:
      test_right_op_list = self.get_op_list(test_ops)
      test_expr = self.combine_expr(test_right_op_list, exprs)
    body, els = [], []
    for child_stmt_detail in stmt_details["body"]:
      body.append(self.formulate_stmt(child_stmt_detail, exprs))
    if "else" in stmt_details:
      for child_stmt_detail in stmt_details["else"]:
        els.append(self.formulate_stmt(child_stmt_detail, exprs))
    if stmt_type == "IF":
      return ast.If(test=test_expr, body=body, orelse=els)
    else:
      return ast.While(test=test_expr, body=body, orelse=els)

  def formulate_for(self, stmt_details, exprs):
    stmt_type = stmt_details["st"]
    new_var = ast.Name(id='i')
    exprs["[LOOP_VAR]"].append(new_var)
    temp_dir = None
    stmt = None
    if stmt_type == "FOR":
      if stmt_details["iter"] == "[NUM_LIST_EXPR]":
        exprs["[NUM_EXPR]"].append(new_var)
        temp_dir = "[NUM_EXPR]"
      if stmt_details["iter"] == "[STR_LIST_EXPR]":
        exprs["[STR_EXPR]"].append(new_var)
        temp_dir = "[STR_EXPR]"
      iter_target = expr_lookup(exprs, stmt_details["iter"])
      stmt = ast.For(target=new_var, iter=iter_target, body=[], orelse=[])
    elif stmt_type == "FOR_RNG":
      exprs["[NUM_EXPR]"].append(new_var)
      temp_dir = "[NUM_EXPR]"
      min_range = ast.Constant(value=stmt_details["iter_min"], kind=None)
      max_range = ast.Constant(value=stmt_details["iter_max"], kind=None)
      iter_range = ast.Call(func=ast.Name(id='range'), args=[min_range, max_range], keywords=[])
      stmt = ast.For(target=new_var, iter=iter_range, body=[], orelse=[])
    elif stmt_type == "FOR_IDX":
      exprs["[NUM_EXPR]"].append(new_var)
      temp_dir = "[NUM_EXPR]"
      iter_target = expr_lookup(exprs, stmt_details["iter"])
      iter_len = ast.Call(func=ast.Name(id='len'), args=[iter_target], keywords=[])
      beg = ast.Constant(value=0, kind=None)
      iter_expr = ast.Call(func=ast.Name(id='range'), args=[beg, iter_len], keywords=[])
      stmt = ast.For(target=new_var, iter=iter_expr, body=[], orelse=[])
    for child_stmt_detail in stmt_details["body"]:
      stmt.body.append(self.formulate_stmt(child_stmt_detail, exprs))
    exprs[temp_dir].remove(new_var)
    return stmt

  def declare_stmt(self, stmt_details, exprs):
    target_type = stmt_details["var_type"]
    new_var = ast.Name(id=stmt_details["var_name"])
    exprs[target_type].append(new_var)
    exprs["[SKIP_DEF]"].append(new_var)
    right = None
    if stmt_details["value"] == "#RAND#":
      right = ast.Constant(value=randint(0, 20), kind=None)
    else:
      right = ast.parse(stmt_details["value"]).body[0]
    stmt = ast.Assign(targets=[new_var], op=ast.Eq(), value=right.value, type_comment=None)
    return stmt

  def method_call(self, stmt_details, exprs):
    right_ops = self.get_op_list(stmt_details["method_call"])
    final_expr = self.combine_expr(right_ops, exprs)
    return ast.Expr(value=final_expr)

  def formulate_stmt(self, stmt_details, exprs):
    stmt_type = stmt_details["st"]
    stmt = None
    if stmt_type == "ASSIGN" or re.match(r"AUGASSIGN_*", stmt_type):
      stmt = self.formulate_assign(stmt_details, exprs)
    if stmt_type == "IF" or stmt_type == "WHILE":
      stmt = self.formulate_if_while(stmt_details, exprs)
    if stmt_type == "DECL":
      stmt = self.declare_stmt(stmt_details, exprs)
    if re.match(r"FOR.*", stmt_type):
      stmt = self.formulate_for(stmt_details, exprs)
    if stmt_type == "METHOD_CALL":
      stmt = self.method_call(stmt_details, exprs)
    if stmt_type == "BREAK":
      stmt = ast.Break()
    if stmt_type == "EVAL":
      stmt = ast.parse(stmt_details["payload"])
    return stmt


    