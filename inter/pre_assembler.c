#include "pre_assembler.h"
#include "definitions.h"
#include "types.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pre_assembler(const char *file_basename) {
    /* Files to work on */
    FILE *user_input = NULL,
         *output_pre_assembled = NULL; /* Pointers to input and output files*/
    int success = 1;

    /* Function variables */
    char line[MAX_LINE_LEN]; /* Buffer to store current line being processed */
    char
        first_word[MAX_MACRO_NAME_LEN + 1]; /* Store the first word - only needs
                                               to be size of macro name */
    int in_macro = 0; /* Flag indicating if we are inside a macro definition */
    char current_macro[MAX_MACRO_NAME_LEN]; /* Buffer for the name of the macro
                                               currently being defined */
    char *current_content; /* Temp pointer for macro content retrieval */
    char new_content[MAX_MACRO_CONTENT_LEN]; /* Buffer for building updated
                                                macro content */
    Macro *existing_macro;

    /* Initialize line counter */
    line_number = 0;

    /* Open input file */
    user_input = fopen(add_ext(file_basename, SRC_F_EXT), "r");
    if (!user_input) {
        fprintf(stderr, "Error: Could not open source file '%s'\n", add_ext(file_basename, SRC_F_EXT));
        return 1;
    }

    /* Open output file */
    output_pre_assembled = fopen(add_ext(file_basename, PAS_F_EXT), "w");
    if (!output_pre_assembled) {
        fprintf(stderr, "Error: Could not create output file '%s'\n", add_ext(file_basename, PAS_F_EXT));
        fclose(user_input);
        return 1;
    }

    fprintf(stderr, "Debug: Starting pre-assembler processing\n");
    fprintf(stderr, "Debug: Source file: %s\n", add_ext(file_basename, SRC_F_EXT));
    fprintf(stderr, "Debug: Destination file: %s\n", add_ext(file_basename, PAS_F_EXT));

    /* Process the file line by line */
    while (fgets(line, MAX_LINE_LEN, user_input) != NULL) {
        line_number++;
        fprintf(stderr, "Debug: Processing line %d: %s", line_number, line);

        extract_first_word(line, first_word);
        fprintf(stderr, "Debug: First word: '%s'\n", first_word);

        /* Macro definition start */
        if (strcmp(first_word, MACRO_START_KW) == 0) {
            fprintf(stderr, "Debug: Found macro start\n");
            if (!extract_macro_name(line, current_macro)) {
                fprintf(stderr, "Error: Invalid macro name at line %d\n", line_number);
                success = 0;
                continue;
            }
            if (!is_valid_macro_name(current_macro)) {
                fprintf(stderr, "Error: Invalid macro name '%s' at line %d\n", current_macro, line_number);
                success = 0;
                continue;
            }
            fprintf(stderr, "Debug: Adding macro '%s'\n", current_macro);
            /* Add macro to table with empty content */
            insert_macro(current_macro, "");
            in_macro = 1;
        }
        /* Macro definition end */
        else if (strcmp(first_word, MACRO_END_KW) == 0) {
            fprintf(stderr, "Debug: Found macro end\n");
            in_macro = 0;
        }
        /* Inside a macro definition - collecting content */
        else if (in_macro) {
            fprintf(stderr, "Debug: Inside macro '%s'\n", current_macro);
            /* Get existing content */
            existing_macro = get_item(macro_table, current_macro);
            if (!existing_macro) {
                fprintf(stderr, "Error: Macro '%s' not found at line %d\n", current_macro, line_number);
                success = 0;
                continue;
            }
            current_content = existing_macro->value;

            /* Create new content by concatenating existing content with the new
             * line */
            if (current_content != NULL) {
                strcpy(new_content, current_content);
            } else {
                new_content[0] = '\0';
            }

            /* Check for buffer overflow */
            if (strlen(new_content) + strlen(line) < MAX_MACRO_CONTENT_LEN) {
                strcat(new_content, line);
                /* Update the macro content */
                insert_macro(current_macro, new_content);
            } else {
                fprintf(stderr, "Error: Macro content too large for '%s'\n",
                        current_macro);
                success = 0;
            }
        }
        /* Regular line or macro invocation */
        else {
            Macro *macro = get_item(macro_table, first_word);
            if (macro != NULL && macro->value != NULL) {
                fprintf(stderr, "Debug: Expanding macro '%s'\n", first_word);
                /* Expand macro */
                fputs(macro->value, output_pre_assembled);
            } else {
                fprintf(stderr, "Debug: Regular line\n");
                /* Regular line */
                fputs(line, output_pre_assembled);
            }
        }
    }

    fprintf(stderr, "Debug: Finished processing. Success = %d\n", success);

    /* close files after finishing process */
    fclose(user_input);
    fclose(output_pre_assembled);

    /* Return success status */
    return success ? 0 : 1;
}

/* extract_macro_name has been moved to utils.c */

void extract_first_word(const char *line, char *word) {
    const char *ptr = line;
    int i;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Copy first word */
    i = 0;
    while (*ptr && !isspace((unsigned char)*ptr) &&
           i < MAX_MACRO_NAME_LEN - 1) {
        word[i++] = *ptr++;
    }
    word[i] = '\0';
}

/**
 * TODO: Keeping for future usage, we might need that for first and second pass
 * @brief Validates a macro name against reserved words
 * Ensures that macro names don't conflict with assembler instructions,
 * directives, or registers.
 * TODO: yuval - move to utils and change name to is_valid_field
 * @param name The macro name to check
 * @return 1 if the name is valid, 0 if invalid
 */
int is_valid_macro_name(const char *name) {
    int i;
    /* List of reserved words that can't be macro names */
    /* TODO use RESERVED_KW */
    const char *reserved_words[] = {
        /* Assembly instructions */
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp",
        "bne", "jsr", "red", "prn", "rts", "stop",

        /* Assembler directives */
        "data", "string", "entry", "extern",

        /* Macro directives */
        MACRO_START_KW, MACRO_END_KW,

        /* Registers */
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",

        /* Terminator */
        NULL};

    /* Check if name is empty */
    if (name[0] == '\0') {
        return 0;
    }

    /* Check against each reserved word */
    for (i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(name, reserved_words[i]) == 0) {
            return 0;
        }
    }

    /* Name is valid */
    return 1;
}
