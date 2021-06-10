cmake_minimum_required(VERSION 3.10)

project(QuestionGenerator VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g -ggdb")

aux_source_directory(src list)
aux_source_directory(src/expression list)
aux_source_directory(src/generator list)
aux_source_directory(src/context list)

add_executable(question_generator ${list})