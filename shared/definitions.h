/**
 * @brief Project-wide definitions for use across shared components
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H 1

/* File handling */
#define SRC_F_EXT ".as"
#define PAS_F_EXT ".am"
#define OBJ_F_EXT ".ob"
#define EXT_F_EXT ".ext"
#define ENT_F_EXT ".ent"
#define LOG_F_LOC "./io/log.txt"

/* Project-wide definitions */
#define MAX_MACRO_NAME_LEN 31
#define MAX_MACRO_CONTENT_LEN 1000

/* Constants */
#define MAX_LINE_LEN 80
#define MAX_SYMBOL_LEN 31
#define MAX_SYMBOLS 256
#define MAX_MEMORY_SIZE 4096

/* Symbol types */
#define SYMBOL_TYPE_CODE 1
#define SYMBOL_TYPE_DATA 2
#define SYMBOL_TYPE_EXTERN 3
#define SYMBOL_TYPE_ENTRY 4

/* Addressing modes */
#define ADDR_MODE_IMMEDIATE 0
#define ADDR_MODE_DIRECT 1
#define ADDR_MODE_INDEX 2
#define ADDR_MODE_REGISTER 3

/* Keywords */
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"

#define RESERVED_KW                                                            \
    {/* Instructions */                                                        \
     "mov",                                                                    \
     "cmp",                                                                    \
     "add",                                                                    \
     "sub",                                                                    \
     "lea",                                                                    \
     "clr",                                                                    \
     "not",                                                                    \
     "inc",                                                                    \
     "dec",                                                                    \
     "jmp",                                                                    \
     "bne",                                                                    \
     "jsr",                                                                    \
     "red",                                                                    \
     "prn",                                                                    \
     "rts",                                                                    \
     "stop",                                                                   \
                                                                               \
     /* Assembler directives */                                                \
     "data",                                                                   \
     "string",                                                                 \
     "entry",                                                                  \
     "extern",                                                                 \
                                                                               \
     /* Macro directives */                                                    \
     MACRO_START_KW,                                                           \
     MACRO_END_KW,                                                             \
                                                                               \
     /* Registers */                                                           \
     "r0",                                                                     \
     "r1",                                                                     \
     "r2",                                                                     \
     "r3",                                                                     \
     "r4",                                                                     \
     "r5",                                                                     \
     "r6",                                                                     \
     "r7",                                                                     \
                                                                               \
     /* Terminator */                                                          \
     NULL}

#endif