# Question Generator
## run
```
mkdir build
cd build
cmake ..
make
```

## features

- [x] Generate the given number of variables
- [ ] Generate the given number of statements
- [ ] The given number of variables that are calculated
- [ ] Appearance ratio of the variables in calculation
- [x] Arithmetic operators: + - * / %
- [x] Appearance of if/for/while
- [x] Compound assignment (i.e. several operators in one calculation)
- [ ] Constraints for conditional expression: numerical values/variables and numerical val/variables and numerical calculation/variables only
- [ ] Complex condition: &&/||
- [x] Nested block
- [x] self-increment/decrement statement
- [x] initialization of variables
- [ ] redundant statement (e.g. a = 1; a = 2;)