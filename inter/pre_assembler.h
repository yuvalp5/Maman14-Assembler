/**
 * @brief Pre-assembler implementation as per P. 40
 */
#ifndef TYPES_H
#define TYPES_H
#include "../shared/types.h"
#endif

#ifndef UTILS_H
#define UTILS_H
#include "../shared/utils.h"
#endif
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include "../wrapper.h"
#endif

/**
 * @brief Pre-assembler method to perform algorythm
 * @param open_loc File to open
 * @param save_loc File to save to
 */
void pre_assembler(char *src, char *dest); // todo int with success/fail output
int macro_definition(char *field);
void replace_macro(FILE *file, const char *line);
int add_macro(FILE *file);
char *get_macro(const char *macro_name);

int is_macro;
