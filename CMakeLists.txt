cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER    "/usr/bin/clang-11.0.3")
set(CMAKE_CXX_COMPILER  "/usr/bin/clang++-11.0.3")
project(QuestionGenerator VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g -ggdb")
include_directories(/usr/local/include)

aux_source_directory(src list)
aux_source_directory(src/expression list)
aux_source_directory(src/generator list)
aux_source_directory(src/context list)

add_executable(question_generator ${list})