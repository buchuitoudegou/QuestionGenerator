import ast
import ast_transform

class QuestionModifier():
  def __init__(self, code_path, config):
    with open(code_path, 'r') as f:
      code = f.read()
      self.tree = ast.parse(code)
      self.config = config
  
  def gen_new_tree(self):
    if self.config["const"]:
      t = ast_transform.ConstTrans()
      t.visit(self.tree)
    if self.config["assignment"]:
      t = ast_transform.AssignmentRewrite()
      t.visit(self.tree)
    if self.config["for"]:
      t = ast_transform.RerangeFor()
      t.visit(self.tree)
    if self.config["branch_sel"]:
      t = ast_transform.BranchRewrite()
      t.visit(self.tree)
    return self.tree
