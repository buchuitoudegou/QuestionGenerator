#include <iostream>
#include "expression/comp_expr.h"
#include "generator/context_gen.h"
#include "generator/code_gen.h"
#include "context/condition.h"
#include <fstream>
#include <toml.hpp>

using std::ofstream;
using toml::parse;

uint16_t gen_flags(const decltype(parse(""))& data) {
  uint16_t flags = 0;
  if (toml::find<int>(data, "IF") == 1) {
    flags |= IF;
  }
  if (toml::find<int>(data, "WHILE") == 1) {
    flags |= WHILE;
  }
  if (toml::find<int>(data, "FOR") == 1) {
    flags |= FOR;
  }
  if (toml::find<int>(data, "CMP_ASGN") == 1) {
    flags |= CMP_ASGN;
  }
  if (toml::find<int>(data, "CPLX_CND") == 1) {
    flags |= CPLX_CND;
  }
  if (toml::find<int>(data, "SLF_INC") == 1) {
    flags |= SLF_INC;
  }
  if (toml::find<int>(data, "RDT_STM") == 1) {
    flags |= RDT_STM;
  }
  if (toml::find<int>(data, "C_CND") == 1) {
    flags |= C_CND;
  }
  if (toml::find<int>(data, "VC_CND") == 1) {
    flags |= VC_CND;
  }
  if (toml::find<int>(data, "VV_CND") == 1) {
    flags |= VV_CND;
  }
  if (toml::find<int>(data, "VE_CND") == 1) {
    flags |= VE_CND;
  }
  if (toml::find<int>(data, "NST_BLK") == 1) {
    flags |= NST_BLK;
  }
  return flags;
}

int main(int argc, const char* argv[]) {
  srand(time(NULL));
  const char* cfg_path = "../default.toml";
  if (argc > 1) {
    cfg_path = argv[1];
  }
  const auto data  = parse(cfg_path);
  auto st_cnt = toml::find<int>(data, "statement_cnt");
  auto v_cnt = toml::find<int>(data, "variable_cnt");
  auto v_app_r = toml::find<double>(data, "variable_app_ratio");
  uint16_t flags = gen_flags(data);
  Context* ctx = generate_context(flags, v_cnt, st_cnt, v_app_r);
  string code = code_generation(ctx);
  ofstream o_file;
  o_file.open("output.cpp");
  o_file << code;
  o_file.close();
  delete ctx;
  return 0;
}