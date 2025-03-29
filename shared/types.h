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

/* Basic DB item structure as K-V pair */
typedef struct Item {
    char *name;
    void *value;
} Item;

/* Basic table structure as array of items */
typedef struct Table {
    int size;
    Item *content[];
} Table;

/* Global counters */
extern int IC;           /* Instruction Counter */
extern int DC;           /* Data Counter */
extern int error_count;  /* Number of errors encountered */
extern int LINE_NUMBER;  /* Current line being processed */
extern int MEMORY_SIZE;  /* Total memory size (IC + DC) */
extern int ICF;          /* Final IC value */
extern int DCF;          /* Final DC value */
extern int symbol_count; /* Number of symbols in symbol table */

/* Global tables */
extern Table macro_table;
extern Table label_table;
extern Table symbol_table;

#endif
