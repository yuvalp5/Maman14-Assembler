/**
 * @brief Shared methods for project
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256
#define MAX_MACRO_NAME_LEN 31      // Maximum length of a macro name
#define MAX_MACRO_CONTENT_LEN 1000 // Maximum length of macro content
#define MAX_MACROS 50 // Maximum number of macros that can be stored
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"

/**
 * @brief Reads a line from a file and returns it as a string
 *
 * @param text text to separate into fields
 * @return line separated into fields
 */
char *line_to_fields(char *text); // TODO: yuval- docs, implementation
