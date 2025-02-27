/**
 * @brief Shared methods for project
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LEN 256
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"

char* ln_to_fields(FILE *file, int ln_num, char *buffer, int size);