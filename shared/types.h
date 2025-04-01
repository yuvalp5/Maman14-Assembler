/**
 * @brief Basic + custom types for use in project
 */

#ifndef TYPES_H
#define TYPES_H 1

#include "definitions.h"
#include <stddef.h>

/* Generic table structure holding dynamically typed content array */
typedef struct {
    int count;
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
/**
 *
 * @param name Name of macro as declared in src
 * @param value Macro content as declared in src
 * @return 0 on success; 1 otherwise
 */
int insert_macro(const char *name, const char *value);

/**
 *
 * @param name Name of label as declared in src
 * @param value Value of macro
 * @return 0 on success; 1 otherwise
 */
int insert_label(const char *name, const int value);
/**
 *
 * @param name Name of label as declared in src
 * @param value Value of macro
 * @param type type of Symbol
 * @return 0 on success; 1 otherwise
 */
int insert_symbol(const char *name, const int value, const int type);

/**
 *
 * @param instance Instance of Table type to search in
 * @param name Name to retrieve content of
 * @return Item content if found; NULL otherwise
 */
void *get_item(const Table *instance, const char *name);

#endif