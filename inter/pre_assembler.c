#include "pre_assembler.h"
#include "../shared/definitions.h"
#include "../shared/types.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pre_assembler(char *src, char *dest) {
    /* Files to work on */
    FILE *user_input = NULL,
         *output_pre_assembled = NULL; /* Pointers to input and output files*/

    /* Function variables */
    char line[MAX_LINE_LEN]; /* Buffer to store current line being processed */
    char
        first_word[MAX_MACRO_NAME_LEN + 1]; /* Store the first word - only needs
                                               to be size of macro name */
    int in_macro = 0; /* Flag indicating if we are inside a macro definition */
    char current_macro[MAX_MACRO_NAME_LEN]; /* Buffer for the name of the macro
                                               currently being defined */
    char *macro_content;   /* Pointer to retrieved macro content */
    char *current_content; /* Temp pointer for macro content retrieval */
    char new_content[MAX_MACRO_CONTENT_LEN]; /* Buffer for building updated
                                                macro content */

    /* Open input file */
    user_input = fopen(src, "r");

    /* Open output file */
    output_pre_assembled = fopen(dest, "w");

    /* Process the file line by line */
    while (fgets(line, MAX_LINE_LEN, user_input) != NULL) {
        extract_first_word(line, first_word); /* extract first word and save in
                                                 first_word variable */

        /* Macro definition start */
        if (strcmp(first_word, MACRO_START_KW) == 0) {
            extract_macro_name(line, current_macro);
            /* Add macro to table with empty content */
            add_string(current_macro, "");
            in_macro = 1;
        }
        /* Macro definition end */
        else if (strcmp(first_word, MACRO_END_KW) == 0) {
            in_macro = 0;
        }
        /* Inside a macro definition - collecting content */
        else if (in_macro) {
            /* Get existing content */
            current_content = get_string(current_macro);

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
                add_string(current_macro, new_content);
            } else {
                fprintf(stderr, "Error: Macro content too large for '%s'\n",
                        current_macro);
            }
        }
        /* Regular line or macro invocation */
        else {
            macro_content = get_string(first_word);
            if (macro_content != NULL) {
                /* Expand macro */
                fputs(macro_content, output_pre_assembled);
            } else {
                /* Regular line */
                fputs(line, output_pre_assembled);
            }
        }
    }

    /* close files after finishing process */
    fclose(user_input);
    fclose(output_pre_assembled);

    /* Free the string table at the end */
    destroy_macro_table();

    /* Return success */
    return 0;
}

int extract_macro_name(const char *line, char *macro_name) {
    const char *ptr = line;
    int i;

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
    i = 0;
    while (*ptr && !isspace((unsigned char)*ptr) &&
           i < MAX_MACRO_NAME_LEN - 1) {
        macro_name[i++] = *ptr++;
    }
    macro_name[i] = '\0';

    /* Validate name is not empty */
    return (i > 0);
}

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
