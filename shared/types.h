/**
 * @brief Basic + custom types for use in project
 */

#ifndef TYPES_H
#define TYPES_H 1

#include "definitions.h"

/* Basic Node structure */
typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* Stack structure */
typedef struct Stack {
    int size;
    Node *head;
} Stack;

/* Basic DB item structure as K-V pair */
typedef struct Item {
    void *name;
    void *value;
    struct Item *next;
} Item;

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
