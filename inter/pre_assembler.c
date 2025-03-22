/**
 * @brief Pre-Assembler implementation for the Assembler project. *
 */
#include "pre_assembler.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*TODO Yuval:
1. modify while loop - extract_first_word out side of the loop to enhance
efficiency
 */

/**
 * @brief Main function for the pre-assembler phase. Executes the pre-aseembling
 * process  *
 * @param src Path to the source assembly file (.as)
 * @param dest Path for the expanded output file (.am)
 * @return 0 on success, 1 on failure
 */
int pre_assembler(char *src, char *dest) {
    /* Files to work on */
    FILE *user_input,
        *output_pre_assembled; /* Pointers to input and output files*/

    /* Function variables */
    char line[MAX_LINE_LEN]; /* Buffer to store current line being processed */
    char first_word[MAX_LINE_LEN]; /* Store the first word of the line*/
    int in_macro = 0; /* Flag indicating if we are inside a macro definition */
    char current_macro[MAX_MACRO_NAME_LEN]; /* Buffer for the name of the macro
                                               currently being defined */

    /* Open input file */
    user_input = fopen(src, "r");

    /* Open output file */
    output_pre_assembled = fopen(dest, "w");

    /* Process the file line by line */
    while (fgets(line, MAX_LINE_LEN, user_input) != NULL) {
        extract_first_word(line, first_word); /* extract first word and save in
                                                 first_word variable */

        /* Handle macro definition start */
        if (strcmp(first_word, MACRO_START_KW) == 0) {
            /* Extract the macro name from the line */
            extract_macro_name(line, current_macro);

            /* Add the new macro to the macro table */
            add_macro(current_macro);

            /* Set flag to indicate we're now inside a macro definition */
            in_macro = 1;

            /* Skip to next line (don't write macro definition line to output)
             */
        }
        /* Handle macro definition end */
        else if (strcmp(first_word, MACRO_END_KW) == 0) {
            /* Reset flag to indicate we're no longer inside a macro definition
             */
            in_macro = 0;

            /* Skip to next line (don't write mcroend line to output) */
        }
        /* Handle content inside macro definition */
        else if (in_macro) {
            /* Add this line to the current macro's content */
            append_to_macro(current_macro, line);

            /* Skip to next line (don't write macro content directly to output)
             */
        } else {
            /* Check if this line starts with a macro name */
            char *macro_content = get_macro_content(first_word);
            if (macro_content != NULL) {
                /* Line starts with a macro name - expand it by writing its
                 * content */
                fputs(macro_content, output_pre_assembled);
            } else {
                /* Not a macro - write the original line as is */
                fputs(line, output_pre_assembled);
            }
        }
    }

    /* close files after finishing process */
    fclose(user_input);
    fclose(output_pre_assembled);

    /* Return success */
    return 0;
}

/**
 * @brief Extracts the macro name from a macro definition line
 *
 * Given a line containing "mcro [name]", this function extracts the
 * name part. It handles whitespace between the "mcro" keyword and
 * the name.
 *
 * @param line The macro definition line (starting with "mcro")
 * @param macro_name Buffer to store the extracted name
 * @return 1 on success, 0 on failure
 */
int extract_macro_name(const char *line, char *macro_name) {
    const char *ptr = line;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Skip "mcro" keyword */
    if (strncmp(ptr, MACRO_START_KW, strlen(MACRO_START_KW)) == 0) {
        ptr += strlen(MACRO_START_KW);
    } else {
        return 0; /* Not a macro definition line */
    }

    /* Skip whitespace after keyword */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Extract the name */
    int i = 0;
    while (*ptr && !isspace((unsigned char)*ptr) &&
           i < MAX_MACRO_NAME_LEN - 1) {
        macro_name[i++] = *ptr++;
    }
    macro_name[i] = '\0';

    /* Validate name is not empty */
    return (i > 0);
}

/**
 * @brief Extracts the first word from a line
 *
 * This is used to identify lines that might be macro invocations
 * or special directives like "mcro" or "mcroend".
 *
 * @param line The input line
 * @param word Buffer to store the extracted first word
 */
void extract_first_word(const char *line, char *word) {
    const char *ptr = line;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Copy first word */
    int i = 0;
    while (*ptr && !isspace((unsigned char)*ptr) &&
           i < MAX_MACRO_NAME_LEN - 1) {
        word[i++] = *ptr++;
    }
    word[i] = '\0';
}

/**
 * @brief Adds a new macro to the macro table
 *
 * Performs validation checks and adds a new macro to the global table.
 * The macro is initially created with empty content, which will be
 * populated by subsequent calls to append_to_macro().
 *
 * @param name Name of the macro to add
 * @return 1 on success, 0 on failure
 */
int add_macro(char *name) {
    /* Check if macro already exists by trying to get it */
    if (get_string(name) != NULL) {
        fprintf(stderr, "Error: Macro '%s' already defined\n", name);
        return 0;
    }

    /* Add the macro to the table with empty content initially */
    return add_string(name, "");
}

/**
 * @brief Appends a line to an existing macro's content
 *
 * This function is called for each line between "mcro" and "mcroend"
 * to build up the macro's content buffer.
 *
 * @param name Name of the macro to append to
 * @param line Line to append to the macro's content
 * @return 1 on success, 0 on failure
 */
int append_to_macro(const char *name, const char *line) {
    /* Get current content */
    char *current_content = get_string(name);

    /* Make sure the macro exists */
    if (current_content == NULL) {
        fprintf(stderr, "Error: Cannot append to undefined macro '%s'\n", name);
        return 0;
    }

    /* Create new combined content */
    char *new_content = malloc(strlen(current_content) + strlen(line) + 1);
    if (new_content == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 0;
    }

    /* Concatenate current content and new line */
    strcpy(new_content, current_content);
    strcat(new_content, line);

    /* Update macro with new content */
    int result = add_string(name, new_content);

    /* Free the temporary buffer */
    free(new_content);

    return result;
}

/**
 * @brief Retrieves a macro's content by name
 *
 * Used during macro expansion to get the content that should replace
 * a macro invocation.
 *
 * @param name Name of the macro to look up
 * @return Pointer to the macro's content, or NULL if macro not found
 */
char *get_macro_content(const char *name) { return get_string(name); }

/**
 * ***** Keeping for future usage, we might need that for first and second pass
 * *******
 * @brief Validates a macro name against reserved words
 *
 * Ensures that macro names don't conflict with assembler instructions,
 * directives, or registers.
 * TODO: yuval - move to utils and change name to is_valid_field
 * @param name The macro name to check
 * @return 1 if the name is valid, 0 if invalid
 */
int is_valid_macro_name(const char *name) {
    /* Check if name is empty */
    if (name[0] == '\0') {
        return 0;
    }

    /* List of reserved words that can't be macro names */
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

    /* Check against each reserved word */
    for (int i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(name, reserved_words[i]) == 0) {
            return 0;
        }
    }

    /* Name is valid */
    return 1;
}