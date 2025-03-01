/**
 * @brief Shared methods for project
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LEN 256
#define MAX_MACRO_NAME_LEN 31 // Maximum length of a macro name
#define MAX_MACRO_CONTENT_LEN 1000 // Maximum length of macro content
#define MAX_MACROS 50 // Maximum number of macros that can be stored
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"

char* ln_to_fields(FILE *file, int ln_num, char *buffer, int size);