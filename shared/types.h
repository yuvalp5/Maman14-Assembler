/**
 * @brief Basic + custom types for use in project
 */

#ifndef TYPES_H
#define TYPES_H 1

#include "definitions.h"

/* Generic table structure holding dynamically typed content array */
typedef struct {
    size_t count;
    void **content;
} Table;

/* Macro structure */
typedef struct {
    char *name;
    char *value;
} Macro;

/* Symbol structure */
typedef struct {
    char *name;
    int symbol_value;
    int symbol_type;
} Symbol;

/* Label structure */
typedef struct {
    char *name;
    int value;
} Label;

/* Global tables */
extern Table *label_table;
extern Table *macro_table;
extern Table *symbol_table;

extern int DC;           /* Data counter initialized to 0 */
extern int IC;           /* Instruction counter initialized to 100 */
extern int symbol_count; /* Number of symbols in the symbol table */
extern int error_count;  /* Number of errors */
extern int line_number;  /* Line number */
extern int ICF;          /* Final IC value */
extern int DCF;          /* Final DC value */
extern int L;            /* Number of words in the instruction */

/* Table functions */
int insert_macro(const char *name, const char *value);
int insert_label(const char *name, const int value);
int insert_symbol(const char *name, const int value, const int type);

#endif
