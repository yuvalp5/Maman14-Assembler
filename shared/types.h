/**
 * @brief Basic + custom types for use in project
 */

#ifndef TYPES_H
#define TYPES_H 1

#include "definitions.h"

/* Stack */

int get_stack_size();

int peek();

int push(int value);

int pop();

int destroy_stack();

/* Label */

int get_label_table_size();

int add_label(char *name, int value);

int get_label(char *name);

void destroy_label_table();

/* Macro */

int get_string_table_size();

int add_string(char *name, char *value);

char *get_string(char *name);

void destroy_string_table();

#endif
