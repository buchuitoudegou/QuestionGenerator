#ifndef __CONDITION_H__
#define __CONDITION_H__

// 16-bit flags controlling the features of the code
#define IF 0x1
#define WHILE 0x2
#define FOR 0x4
#define CMP_ASGN 0x8
#define CPLX_CND 0x10
#define SLF_INC 0x20
#define RDT_STM 0x40
#define C_CND 0x80
#define VC_CND 0x100
#define VV_CND 0x200
#define NST_BLK 0x400

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
#define INV_VAR_DECL 0x4
#define INV_PAREN 0x8
#define DEAD_LOOP 0x10

#endif