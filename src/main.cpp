#include <iostream>
#include "expression/comp_expr.h"
#include "generator/context_gen.h"
#include "generator/code_gen.h"
#include "context/condition.h"
#include <fstream>
#include <toml.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <iterator>

using std::istream_iterator;
using std::ofstream;
using std::ifstream;
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

void dup_option(vector<vector<int>>& options) {
  if (options.size() == 0) {
    for (int i = 0; i < 3; ++i) {
      int a1 = rand() % 100, a2 = rand() % 100;
      vector<int> tmp{a1, a2};
      options.push_back(tmp);
    }
  } else {
    for (int i = 0; i < 2; ++i) {
      int a1 = i == 0? options[0][0]: rand() % 100;
      int a2 = i == 1? options[0][1]: rand() % 100;
      vector<int> tmp{a1, a2};
      options.push_back(tmp);
    }
  }
}

int main(int argc, const char* argv[]) {
  srand(time(NULL));
  const char* cfg_path = "../default.toml";
  if (argc > 1) {
    cfg_path = argv[1];
  }
  /*
   *  generate code w.r.t the input  
   */
  const auto data  = parse(cfg_path);
  auto st_cnt = toml::find<int>(data, "statement_cnt");
  auto v_cnt = toml::find<int>(data, "variable_cnt");
  auto v_app_r = toml::find<double>(data, "variable_app_ratio");
  uint16_t flags = gen_flags(data);
  Context* ctx = generate_context(flags, v_cnt, st_cnt, v_app_r);
  string code = code_generation(ctx);
  ofstream o_file("output.cpp");
  o_file << code;
  o_file.close();
  delete ctx;
  /*
   *  compile the generated code and get the output
   */
  int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU); // create output file
  auto pid = fork(); // compile the code in a child process
  if (pid == 0) {
    execlp("g++", "g++", "output.cpp", "-o", "output", "-w", NULL);
  }
  while (waitpid(pid, NULL, WNOHANG) == 0); // wait for the child ending
  int save_stdout = dup(STDOUT_FILENO); // save stdout
  close(STDOUT_FILENO); // redirect the standard output to a file (printf->file)
  dup(fd); // redirecting
  pid = fork();
  if (pid == 0) {
    execlp("./output", "./output", NULL); // executing the binary file
  }
  while (waitpid(pid, NULL, WNOHANG) == 0); // wait for the child ending
  close(fd); // finish processing
  close(STDOUT_FILENO); // finish processing
  dup2(save_stdout, STDOUT_FILENO); // re-open the stdout
  close(save_stdout);
  /*
   * generate the question and the distractors with the output
   */
  ifstream ifs("out.txt");
  istream_iterator<int> it_ifs(ifs), eof;
  vector<int> answer(it_ifs, eof); // correct answer
  string question = "What is the output of the following code: \n";
  question += code + "\n";
  vector<vector<int>> options;
  if (answer.size() > 0) {
    options.push_back(answer);
  }
  dup_option(options);
  for (int i = 0; i < options.size(); ++i) {
    question.push_back('A' + i);
    question += ". ";
    for (auto& ans: options[i]) {
      question += std::to_string(ans) + " ";
    }
    question += "\n";
  }
  question += "D. RUNTIME ERROR\n";
  ofstream q_str("question.txt", ofstream::trunc);
  q_str << question << std::endl;
  q_str.close();
  ifs.close();
  return 0;
}