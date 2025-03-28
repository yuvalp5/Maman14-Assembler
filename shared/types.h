/**
 * @brief Basic + custom types for use in project
 */

#ifndef TYPES_H
#define TYPES_H 1

#include "definitions.h"

/* Symbol types */
#define SYMBOL_TYPE_CODE 1
#define SYMBOL_TYPE_DATA 2
#define SYMBOL_TYPE_EXTERN 3
#define SYMBOL_TYPE_ENTRY 4

/* Global counters */
extern int IC;           /* Instruction Counter */
extern int DC;           /* Data Counter */
extern int error_count;  /* Number of errors encountered */
extern int LINE_NUMBER;  /* Current line being processed */
extern int MEMORY_SIZE;  /* Total memory size (IC + DC) */
extern int ICF;          /* Final IC value */
extern int DCF;          /* Final DC value */
extern int symbol_count; /* Number of symbols in symbol table */

/* Symbol table */
extern Symbol symbol_table[MAX_SYMBOLS];

/* Basic Item structure */
typedef struct Item {
    char *name;
    char *value;
    struct Item *next;
} Item;

/* Table structure */
typedef struct {
    Item *content;
    int size;
} Table;

/* Global tables */
extern Table label_table;
extern Table string_table;
extern Table macro_table;

/* Stack functions */
int is_empty(void);
int get_stack_size(void);
int peek(void);
int push(int value);
int pop(void);
int destroy_stack(void);

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
