/**
 * @brief Pre-assembler implementation
 * Handles macro expansion in source files
 */

#include "definitions.h"
#include "types.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Add explicit declaration for strdup if needed */
#if defined(__STRICT_ANSI__)
char *strdup(const char *s);
#endif

int process_line_pre_assembler(char *line, int *in_macro, char *current_macro, char *macro_content) {
    char first_word[MAX_MACRO_NAME_LEN + 1];
    char *ptr = line;
    int success = 1;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Extract first word */
    sscanf(ptr, "%s", first_word);

    /* Check for macro definition start */
    if (strcmp(first_word, MACRO_START_KW) == 0) {
        if (*in_macro) {
            printf("Error: Nested macro definition at line %d\n", line_number);
            success = 0;
        } else {
            if (!extract_macro_name(ptr, current_macro)) {
                printf("Error: Invalid macro name at line %d\n", line_number);
                success = 0;
            } else {
                *in_macro = 1;
                macro_content[0] = '\0';
            }
        }
    }
    /* Check for macro definition end */
    else if (strcmp(first_word, MACRO_END_KW) == 0) {
        if (!*in_macro) {
            printf("Error: %s without matching %s at line %d\n", MACRO_END_KW, MACRO_START_KW, line_number);
            success = 0;
        } else {
            /* Add macro to table */
            if (insert_macro(current_macro, macro_content) != 0) {
                printf("Error: Memory allocation failed at line %d\n", line_number);
                success = 0;
            }
            *in_macro = 0;
        }
    }
    /* Inside macro definition */
    else if (*in_macro) {
        /* Append line to macro content */
        if (strlen(macro_content) + strlen(line) < MAX_MACRO_CONTENT_LEN) {
            strcat(macro_content, line);
        } else {
            printf("Error: Macro content too large at line %d\n", line_number);
            success = 0;
        }
    }
    /* Check for macro usage */
    else {
        Macro *macro = get_item(macro_table, first_word);
        if (macro) {
            /* Replace macro usage with its content */
            strcpy(line, macro->value);
        }
    }

    return success;
}

/* The extract_macro_name function was removed to eliminate the duplicate definition.
   Using the version from pre_assembler.c instead, which is made available through pre_assembler.h */

int is_empty_or_comment(const char *line) {
    const char *ptr = line;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Check if line is empty or starts with ';' */
    return (*ptr == '\0' || *ptr == ';');
}
 