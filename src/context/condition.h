#ifndef __CONDITION_H__
#define __CONDITION_H__

// 16-bit flags controlling the features of the code
#define LOGIC_COMP 0x1
#define BRANCH_SEL 0x2
#define MACRO 0x4
#define STATIC_VAR 0x8
#define WHILE_LOOP 0x10
#define FOR_LOOP 0x20
#define FUNC_CALL 0x40
#define FUNC_DEF 0x80

// question generation: question form
#define MCQ 0x1
#define FILL_BLK 0x2
#define REORDER 0x4

// question generation: question type
#define PRED_OUTPUT 0x1
#define IDT_SE 0x2
#define IDT_RE 0x4

// question generation: error
#define IDX_OUT_OF_BOUND 0x1
#define MIS_SEMI 0x2

#endif