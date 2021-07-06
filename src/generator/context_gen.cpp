#include "context_gen.h"

using std::map;
using std::make_pair;

Context* generate_context(uint16_t flags, int var_num, int st_num, double app_ratio) {
  Context* ctx = new Context();
  ctx->define_main();
  for (int i = 0; i < var_num; ++i) {
    decl_var(ctx, INT, "main");
  }
  vector<Variable> vars = ctx->get_vars(INT, "main");
  int drop = (int)(vars.size() * (1 - app_ratio));
  while (drop) {
    int drop_idx = rand() % vars.size();
    vars.erase(vars.begin() + drop_idx);
    drop --;
  }
  if (no_block(flags)) {
    // generate arth statement in main
    for (int i = 0; i < st_num; ++i) {
      bool is_comp = (flags & CMP_ASGN) && (rand() % 2? true: false);
      Expr* arth = gen_arth(vars, is_comp);
      ctx->insert_func_norm_expr("main", arth);
    }
  } else {
    // insert block
    vector<Expr*> scopes;
    if (flags & WHILE) {
      scopes.push_back(gen_while(vars, flags));
    }
    if (flags & FOR) {
      scopes.push_back(gen_for(vars));
    }
    if (flags & IF) {
      scopes.push_back(gen_if(vars, flags));
    }
    vector<Expr*> shuffles;
    map<Expr*, Expr*> scope_nst_block;
    while (scopes.size()) {
      auto rand_idx = rand() % scopes.size();
      auto it = scopes.begin() + rand_idx;
      if ((*it)->type == SEL) {
        shuffles.push_back(*it);
        if (rand() % 2) {
          shuffles.push_back(gen_else());
        }
      } else {
        if ((flags & NST_BLK) && rand() % 2) {
          scope_nst_block.insert(make_pair(*it, gen_if(vars, flags)));
        }
        shuffles.push_back(*it);
      }
      scopes.erase(it);
    }
    vector<int> arth_for_scopes(shuffles.size() + 2, 0);
    assert(st_num >= arth_for_scopes.size() - 2);
    for (int i = 1; i < arth_for_scopes.size() - 1; i++) {
      arth_for_scopes[i] = 1;
    }
    int rst_num = st_num - arth_for_scopes.size() + 2;
    while (rst_num --) {
      int rand_idx = rand() % arth_for_scopes.size();
      arth_for_scopes[rand_idx] ++;
    }
    for (int i = 0; i < arth_for_scopes[0]; ++i) {
      bool is_comp = (flags & CMP_ASGN) && (rand() % 2? true: false);
      Expr* arth = gen_arth(vars, is_comp);
      if ((flags & RDT_STM) && rand() % 10 == 1) {
        ctx->insert_func_norm_expr("main", arth); // redundant
      }
      ctx->insert_func_norm_expr("main", arth);
    }
    // insert for scopes
    for (int i = 1; i < arth_for_scopes.size() - 1; ++i) {
      Expr* scope_expr = shuffles[i-1];
      vector<Expr*> exprs;
      Expr* nst_scope = nullptr;
      if (scope_nst_block.find(scope_expr) != scope_nst_block.end()) {
        nst_scope = scope_nst_block.find(scope_expr)->second;
      }
      ctx->insert_scope_expr(scope_expr, "main");
      for (int j = 0; j < arth_for_scopes[i]; ++j) {
        bool is_comp = (flags & CMP_ASGN) && (rand() % 2? true: false);
        Expr* arth = gen_arth(vars, is_comp);
        exprs.push_back(arth);
      }
      if (nst_scope != nullptr) {
        int mid = rand() % exprs.size();
        vector<Expr*> a1(exprs.begin(), exprs.begin() + mid);
        ctx->insert_exprs_to_scope(scope_expr, "main", a1);
        ctx->insert_scope_expr(nst_scope, "main", scope_expr);
        vector<Expr*> a2(exprs.begin() + mid, exprs.end());
        ctx->insert_exprs_to_scope(nst_scope, "main", a2);
      } else {
        ctx->insert_exprs_to_scope(scope_expr, "main", exprs);
      }
    }
    // insert tail
    for (int i = 0; i < arth_for_scopes[arth_for_scopes.size() - 1]; ++i) {
      bool is_comp = (flags & CMP_ASGN) && (rand() % 2? true: false);
      Expr* arth = gen_arth(vars, is_comp);
      if ((flags & RDT_STM) && rand() % 10 == 1) {
        ctx->insert_func_norm_expr("main", arth); // redundant
      }
      ctx->insert_func_norm_expr("main", arth);
    }
  }
  Expr* output = new OutExpr(vars);
  ctx->insert_func_norm_expr("main", output);
  ctx->insert_func_ret_expr("main", new RetExpr("0"));
  return ctx;
}

bool decl_var(Context* ctx, VarType vt, string func_name) {
  ctx->def_func_var(func_name, vt);
}

Expr* gen_arth(vector<Variable>& vars, bool is_compound) {
  // left: assignment expr or selfcomp assignment expr
  // right: var or self-increment expr
  int arg_num = rand() % 2 + 2;
  int is_scomp = (rand() % 2? true: false) && is_compound;
  CompType scomp_type = PLUS;
  if (is_scomp) {
    scomp_type = static_cast<CompType>(PLUS + rand() % 4);
  }
  Expr* prev = nullptr;
  for (int i = 0; i < arg_num; ++i) {
    int is_sc = rand() % 2;
    bool is_const = rand() % 2;
    auto rand_var = vars[rand() % vars.size()];
    Expr* temp;
    if (is_compound && is_sc && !is_const) {
      CompType ct = static_cast<CompType>(PLUS + rand() % 2);
      bool is_back = rand() % 2? true: false;
      temp = new SelfCompExpr(rand_var, ct, is_back);
    } else {
      if (is_const) {
        char buffer[3] = {0};
        int const_num = rand() % 20;
        sprintf(buffer, "%d", const_num);
        temp = new ConstExpr(buffer);
      } else {
        temp = new VarExpr(rand_var);
      }
    }
    if (i == 0) {
      prev = temp;
    }
    if (i != 0) {
      CompType ct = static_cast<CompType>(PLUS + rand() % 5);
      auto new_comp = new CompExpr(prev, temp, ct);
      prev = new_comp;
    }
  }
  auto dst = vars[rand() % vars.size()];
  if (is_scomp) {
    return new SelfCompAssignExpr(dst, scomp_type, prev);
  } else {
    return new AssignExpr(dst, prev);
  }
}

WhileExpr* gen_while(vector<Variable>& vars, uint16_t flags) {
  // To prevent the generated code from dead loop
  // the cond in while expression is restricted!
  CompareType ct = rand() % 2 == 0? LARGER: NO_LESS;
  bool filters[] = {
    static_cast<bool>(flags & VC_CND),
    static_cast<bool>(flags & VV_CND),
  };
  int sel = select_from(filters, 2);
  Expr* right_expr, *left_expr;
  if (sel == 0 || sel == -1) {
    // var and const
    int num = rand() % 20;
    char numc[3] = {0};
    sprintf(numc, "%d", num);
    right_expr = new ConstExpr(numc);
    left_expr = new SelfCompExpr(vars[rand() % vars.size()], SUB);
  } else {
    right_expr = new VarExpr(vars[rand() % vars.size()]);
    left_expr = new SelfCompExpr(vars[rand() % vars.size()], SUB);
  }
  auto cond = new BoolExpr(left_expr, right_expr, ct);
  return new WhileExpr(cond);
}

ForExpr* gen_for(vector<Variable>& vars) {
  Expr* init = new DeclExpr("i", INT, "0");
  int num = rand() % 10;
  char numc[3] = {0};
  sprintf(numc, "%d", num);
  Expr* ctrl = new BoolExpr(new VarExpr(Variable("i", INT)), new ConstExpr(numc), LESS);
  Expr* iter = new SelfCompExpr(Variable("i", INT), PLUS);
  ForExpr* for_expr = new ForExpr(init, ctrl, iter);
  return for_expr;
}

IfExpr* gen_if(vector<Variable>& vars, uint16_t flags) {
  bool filters[] = {
    static_cast<bool>(flags & C_CND),
    static_cast<bool>(flags & VC_CND),
    static_cast<bool>(flags & VV_CND),
    static_cast<bool>(flags & VE_CND)
  };
  bool cplx = (flags & CPLX_CND) && (rand() % 2 == 1)? true: false;
  int sel = select_from(filters, 4);
  if (sel == 0 || sel == -1) {
    // const and const
    int left_num = rand() % 20;
    int right_num = rand() % 20;
    char l1[3]={0}, l2[3]={0};
    sprintf(l1, "%d", left_num);
    sprintf(l2, "%d", right_num);
    ConstExpr* left_expr = new ConstExpr(l1);
    ConstExpr* right_expr = new ConstExpr(l2);
    auto compare_type = static_cast<CompareType>(EQUAL+ rand() % 5);
    auto cond = new BoolExpr(left_expr, right_expr, compare_type);
    return new IfExpr(cond);
  }
  if (sel == 1) {
    // const and var
    // complex condition could work
    vector<BoolExpr*> bexprs;
    Expr* cond = nullptr;
    int bnum = cplx? 2: 1;
    for (int i = 0; i < bnum; ++i) {
      int right_num = rand() % 20;
      char l2[3]={0};
      sprintf(l2, "%d", right_num);
      ConstExpr* right_expr = new ConstExpr(l2);
      auto rand_var = vars[rand() % vars.size()];
      VarExpr* left_expr = new VarExpr(rand_var);
      auto compare_type = static_cast<CompareType>(EQUAL + rand() % 5);
      bexprs.push_back(new BoolExpr(left_expr, right_expr, compare_type));
    }
    if (cplx) {
      CompareType ct = rand() % 2? OR: AND;
      cond = new BoolExpr(bexprs[0], bexprs[1], ct);
    } else {
      cond = bexprs[0];
    }
    return new IfExpr(cond);
  }
  if (sel == 2) {
    // var and var
    // complex condition could work
    vector<BoolExpr*> bexprs;
    Expr* cond = nullptr;
    int bnum = cplx? 2: 1;
    for (int i = 0; i < bnum; ++i) {
      VarExpr* v1_expr = new VarExpr(vars[rand() % vars.size()]);
      VarExpr* v2_expr = new VarExpr(vars[rand() % vars.size()]);
      auto compare_type = static_cast<CompareType>(EQUAL + rand() % 5);
      bexprs.push_back(new BoolExpr(v1_expr, v2_expr, compare_type));
    }
    if (cplx) {
      CompareType ct = rand() % 2? OR: AND;
      cond = new BoolExpr(bexprs[0], bexprs[1], ct);
    } else {
      cond = bexprs[0];
    }
    return new IfExpr(cond);
  }
  if (sel == 3) {
    // var and expr
    auto v1 = vars[rand() % vars.size()];
    auto v2 = vars[rand() % vars.size()];
    VarExpr* v1_expr = new VarExpr(v1);
    VarExpr* v2_expr = new VarExpr(v2);
    Expr* temp;
    if (rand() % 2) {
      int right_num = rand() % 20;
      char l2[3]={0};
      sprintf(l2, "%d", right_num);
      temp = new ConstExpr(l2);
    } else {
      temp = new VarExpr(vars[rand() % vars.size()]);
    }
    auto compare_type = static_cast<CompareType>(EQUAL + rand() % 5);
    CompType ct = static_cast<CompType>(PLUS + rand() % 5);
    CompExpr* comp_expr = new CompExpr(v2_expr, temp, ct);
    BoolExpr* be = new BoolExpr(v1_expr, comp_expr, compare_type);
    return new IfExpr(be);
  }
}

ElseExpr* gen_else() {
  return new ElseExpr();
}

int select_from(bool filters[], int size) {
  bool all_false = true;
  for (int i = 0; i < size; ++i) {
    if (filters[i]) {
      all_false = false;
    }
  }
  if (all_false) {
    return -1;
  }
  int step = rand() % (size + 1);
  int idx = 0;
  int sel = idx;
  while (step > 0) {
    if (filters[idx]) {
      step --;
      sel = idx;
      idx = (idx + 1) % size;
    } else {
      idx = (idx + 1) % size;
    }
  }
  return sel;
}