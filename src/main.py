import ast
# import random
# import astunparse

# with open('src.py', 'r') as f:
#     code = f.read()
#     tree = ast.parse(code)
#     for comp in tree.body:
#         if isinstance(comp, ast.Assign) and isinstance(comp.value, ast.Constant):
#             # initialization statement
#             # change the initial value of the variable
#             comp.value = ast.Constant(value=random.randint(100, 200), kind=None)
#     # parse the tree to python
#     # todo: binding the tree with different languages and generate different kinds of code
#     new_code = astunparse.unparse(tree) # one of the alternatives
#     with open('ret.py', 'w') as f:
#         f.write(new_code)

import generator

generator.gen_ast('')
# print(ast.dump(ast.parse('for i in range(0, 10):\n  print(i)')))