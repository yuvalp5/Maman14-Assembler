/**
 * @brief Basic + custom types for use in project
 */

#ifndef TYPES_H
#define TYPES_H 1

#include "definitions.h"

/* Basic DB item structure as K-V pair */
typedef struct Item {
    void *name;
    void *value;
    struct Item *next;
} Item;

/* Symbol table structure */
typedef struct Symbol {
    char symbol_name[MAX_SYMBOL_LEN + 1];
    int symbol_value;
    int symbol_type;
} Symbol;

/* Basic table structure as array of items */
typedef struct Table {
    int size;
    Item *content;
} Table;

extern int DC;           /* Data counter initialized to 0 */
extern int IC;           /* Instruction counter initialized to 100 */
extern int symbol_count; /* Number of symbols in the symbol table */
extern int error_count;  /* Number of errors */
extern int line_number;  /* Line number */
extern int ICF;          /* Final IC value */
extern int DCF;          /* Final DC value */
extern int L;            /* Number of words in the instruction */

/* Global tables */
/* TODO: implement as item arrays */
extern Table label_table;
extern Table string_table;
extern Table macro_table;
extern Symbol symbol_table[MAX_SYMBOLS];

/* Label table functions */
int get_label_table_size(void);
int add_label(char *name, int value);
int get_label(char *name);
void destroy_label_table(void);

/* String table functions */
int get_string_table_size(void);
int add_string(char *name, char *value);
char *get_string(char *name);
void destroy_string_table(void);

/* Custom string functions */
char *my_strdup(const char *str);

#endif
