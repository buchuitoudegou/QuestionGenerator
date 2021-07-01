# Question Generator

## dependencies
1. [toml11](https://github.com/ToruNiina/toml11)

## prepare
```
git clone https://github.com/ToruNiina/toml11.git
cd toml11
mkdir build & cd build
cmake ..
make & make install
```

## run
```
mkdir build
cd build
cmake ..
make
```

## features

- [x] Generate the given number of variables
- [x] Generate the given number of statements
- [x] Appearance ratio of the variables in calculation
- [x] Arithmetic operators: + - * / %
- [x] Appearance of if/for/while
- [x] Compound assignment (i.e. several operators in one calculation)
- [x] Constraints for conditional expression: numerical values/variables and numerical val/variables and numerical calculation/variables only
- [x] Complex condition: &&/||
- [x] Nested block
- [x] self-increment/decrement statement
- [x] initialization of variables
- [x] redundant statement (e.g. a = 1; a = 2;)