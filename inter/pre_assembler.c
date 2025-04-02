#include "pre_assembler.h"
#include "definitions.h"
#include "types.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Function to process a line in the pre-assembler as a master method */
int pre_assembler(const char *file_basename) {
    /* Files to work on */
    FILE *user_input = NULL,
         *output_pre_assembled = NULL; /* Pointers to input and output files*/
    int success = 1;

    /* Function variables */
    char line[MAX_LINE_LEN]; 
    char first_word[MAX_MACRO_NAME_LEN + 1];
    int in_macro = 0; 
    char current_macro[MAX_MACRO_NAME_LEN]; 
    char *current_content; 
    char new_content[MAX_MACRO_CONTENT_LEN]; 
    Macro *existing_macro;

    /* Initialize line counter */
    line_number = 0;

    /* Open input file */
    user_input = fopen(add_ext(file_basename, SRC_F_EXT), "r");
    if (!user_input) {
        printf("[PRE-ASSEMBLER:] Error: Could not open source file '%s'\n",
               add_ext(file_basename, SRC_F_EXT));
        return 1;
    }

    /* Open output file */
    output_pre_assembled = fopen(add_ext(file_basename, PAS_F_EXT), "w");
    if (!output_pre_assembled) {
        printf("[PRE-ASSEMBLER:] Error: Could not create output file '%s'\n",
               add_ext(file_basename, PAS_F_EXT));
        fclose(user_input);
        return 1;
    }
    /* Print debug information */
    printf("[PRE-ASSEMBLER:] Starting pre-assembler processing\n");
    printf("[PRE-ASSEMBLER:] Source file: %s\n",
           add_ext(file_basename, SRC_F_EXT));
    printf("[PRE-ASSEMBLER:] Destination file: %s\n",
           add_ext(file_basename, PAS_F_EXT));

    /* Process the file line by line */
    while (fgets(line, MAX_LINE_LEN, user_input) != NULL) {
        line_number++;
        printf("[PRE-ASSEMBLER:] [PRE-ASSEMBLER:] Processing line %d: %s",
               line_number, line);

        extract_first_word(line,
                           first_word); /* Extract the first word to identify if
                                           it's a macro definition  */
        printf("[PRE-ASSEMBLER:] First word: '%s'\n", first_word);

        /* if we found a macro definition */
        if (strcmp(first_word, MACRO_START_KW) == 0) {
            printf("[PRE-ASSEMBLER:] Found macro start\n");
            if (!extract_macro_name(line, current_macro)) {
                printf(
                    "[PRE-ASSEMBLER:] Error: Invalid macro name at line %d\n",
                    line_number);
                success = 0;
                continue;
            }
            if (!is_reserved_word(current_macro)) {
                printf("[PRE-ASSEMBLER:] Error: Invalid macro name '%s' at "
                       "line %d\n",
                       current_macro, line_number);
                success = 0;
                continue;
            }
            printf("[PRE-ASSEMBLER:] Adding macro '%s'\n", current_macro);
            /* Add macro to table with empty content */
            insert_macro(current_macro, "");
            in_macro = 1;
        }
        /* Macro definition end */
        else if (strcmp(first_word, MACRO_END_KW) == 0) {
            printf("[PRE-ASSEMBLER:] Found macro end\n");
            in_macro = 0;
        }
        /* Inside a macro definition - collecting content */
        else if (in_macro) {
            printf("[PRE-ASSEMBLER:] macro '%s'\n", current_macro);
            /* Get existing content */
            existing_macro = get_item(macro_table, current_macro);
            if (!existing_macro) {
                printf(
                    "[PRE-ASSEMBLER:] Error: Macro '%s' not found at line %d\n",
                    current_macro, line_number);
                success = 0;
                continue;
            }
            current_content =
                existing_macro
                    ->value; /* Get the existing content of the macro */

            /* Create new content by concatenating existing content with the new
             * line */
            if (current_content != NULL) {
                strcpy(new_content, current_content);
            } else {
                new_content[0] = '\0';
            }

            /* Check for buffer overflow */
            if (strlen(new_content) + strlen(line) < MAX_MACRO_CONTENT_LEN) {
                strcat(new_content, line); /* Concatenate the new line to the
                                              existing content */
                /* Update the macro content */
                insert_macro(current_macro, new_content);
            } else {
                printf("[PRE-ASSEMBLER:] Error: Macro content too large for "
                       "'%s'\n",
                       current_macro);
                success = 0;
            }
        }
        /* Regular line or macro invocation */
        else {
            Macro *macro = get_item(macro_table, first_word);
            if (macro != NULL && macro->value != NULL) {
                printf("[PRE-ASSEMBLER:] Expanding macro '%s'\n", first_word);
                /* Expand macro by writing its content to output */
                fputs(macro->value, output_pre_assembled);
            } else {
                printf("[PRE-ASSEMBLER:] Regular line\n");
                /* Copy regular line directly to output */
                fputs(line, output_pre_assembled);
            }
        }
    }

    printf("[PRE-ASSEMBLER:] Finished processing. Success = %d\n", success);

    /* close files after finishing process */
    fclose(user_input);
    fclose(output_pre_assembled);

    /* Return success status */
    return success ? 0 : 1;
}

/* extract_macro_name has been moved to utils.c */

/* Function to extract the first word from a line */
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
