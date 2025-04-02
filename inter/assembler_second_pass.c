/**
 * @brief Second pass implementation
 * Handles final code generation and output file creation
 */

#include "assembler_second_pass.h"
#include "definitions.h"
#include "types.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Function declarations */
int search_command_in_table(const char *command);
int get_addressing_mode(const char *operand);

/* Global variables from types.h */
extern int *code_memory;    /* Memory for storing machine code */
extern int DC;              /* Data counter */
extern int IC;              /* Instruction counter */
extern int ICF;             /* Final IC value */
extern int DCF;             /* Final DC value */
extern int L;               /* Number of words in the instruction */
extern int symbol_count;    /* Number of symbols in the symbol table */
extern int error_count;     /* Number of errors */
extern int line_number;     /* Line number */
extern Table *symbol_table; /* Symbol table */

/**
 * @brief Master function for second pass
 * @param object_file Path to the object file to be created
 * @return 0 on success, 1 on failure
 */
int second_pass(const char *object_file) {
    FILE *input_file = NULL;  /* Input file */
    FILE *output_file = NULL; /* Output file */
    FILE *ext_file = NULL;    /* External file */
    FILE *ent_file = NULL;    /* Entry file */
    char line[MAX_LINE_LEN];  /* Line to be processed */
    char *field;              /* Field to be used for parsing */
    char *symbol_name;        /* Symbol name */
    Symbol *symbol;           /* Symbol instance */
    int i;                    /* Index */
    int has_entry = 0;        /* Entry flag */
    int has_extern = 0;       /* Extern flag */

    /* Open input file (.am file after macro expansion) */
    input_file = fopen(add_ext(object_file, PAS_F_EXT), "r");
    if (!input_file) {
        printf("[SECOND PASS:] Error: Unable to open input file at %s\n",
               add_ext(object_file, PAS_F_EXT));
        return 1;
    }

    /* First scan to identify entry and extern symbols */
    while (fgets(line, MAX_LINE_LEN, input_file) != NULL) {
        line_number++;

        /* Skip empty lines and comments */
        if (is_empty_or_comment(line)) {
            continue;
        }

        /* Get first field */
        field = strtok(line, " \t\n");
        if (!field)
            continue;

        /* Check for entry directive */
        if (strcmp(field, ".entry") == 0) {
            has_entry = 1;
            symbol_name = strtok(NULL, " \t\n");
            if (symbol_name) {
                symbol = get_item(symbol_table, symbol_name);
                if (symbol) {
                    /* Mark symbol as entry but preserve its original type */
                    if (symbol->symbol_type != SYMBOL_TYPE_EXTERN) {
                        symbol->symbol_type |= SYMBOL_TYPE_ENTRY;
                    } else {
                        printf("[SECOND PASS:] Error: Symbol cannot be both "
                               "entry and extern");
                        error_count++;
                    }
                } else {
                    printf("[SECOND PASS:] Error: Entry symbol %s not found",
                           symbol_name);
                    error_count++;
                }
            }
        }
        /* Check for extern directive */
        else if (strcmp(field, ".extern") == 0) {
            has_extern = 1;
            symbol_name = strtok(NULL, " \t\n");
            if (symbol_name) {
                symbol = get_item(symbol_table, symbol_name);
                if (!symbol) {
                    /* Add external symbol if not already in table */
                    if (insert_symbol(symbol_name, 0, SYMBOL_TYPE_EXTERN) !=
                        0) {
                        printf("[SECOND PASS:] Error: Failed to add external "
                               "symbol %s\n",
                               symbol_name);
                        error_count++;
                    }
                }
            }
        }
    }

    /* Create output files */
    output_file = fopen(add_ext(object_file, OBJ_F_EXT), "w");
    if (!output_file) {
        printf("[SECOND PASS:] Error: Unable to create object file at %s\n",
               add_ext(object_file, OBJ_F_EXT));
        fclose(input_file);
        return 1;
    }

    /* Write header */
    fprintf(output_file, "\t%d %d\n", ICF - 100, DCF);

    /* Write code segment */
    for (i = 100; i < ICF; i++) {
        fprintf(output_file, "%04d\t%06X\n", i - 100,
                code_memory[i] & 0xFFFFFF);
    }

    /* Write data segment */
    for (i = 0; i < DCF; i++) {
        fprintf(output_file, "%04d\t%06X\n", ICF - 100 + i,
                code_memory[ICF + i] & 0xFFFFFF);
    }

    fclose(output_file);

    /* Create entry file if needed */
    if (has_entry) {
        ent_file = fopen(add_ext(object_file, ENT_F_EXT), "w");
        if (!ent_file) {
            printf("[SECOND PASS:] Error: Unable to create entry file at %s\n",
                   add_ext(object_file, ENT_F_EXT));
            fclose(input_file);
            return 1;
        }

        /* Write entry symbols */
        for (i = 0; i < symbol_count; i++) {
            symbol = symbol_table->content[i];
            if (symbol && (symbol->symbol_type & SYMBOL_TYPE_ENTRY)) {
                fprintf(ent_file, "%s\t%04d\n", symbol->name,
                        symbol->symbol_value - 100);
            }
        }

        fclose(ent_file);
    }

    /* Create extern file if needed */
    if (has_extern) {
        ext_file = fopen(add_ext(object_file, EXT_F_EXT), "w");
        if (!ext_file) {
            printf(
                "[SECOND PASS:] Error: Unable to create external file at %s\n",
                add_ext(object_file, EXT_F_EXT));
            fclose(input_file);
            return 1;
        }

        /* Write external symbols */
        for (i = 0; i < symbol_count; i++) {
            symbol = symbol_table->content[i];
            if (symbol && (symbol->symbol_type & SYMBOL_TYPE_EXTERN)) {
                fprintf(ext_file, "%s\t%04d\n", symbol->name,
                        symbol->symbol_value - 100);
            }
        }

        fclose(ext_file); /* Close the external file */
    }

    fclose(input_file);     /* Close the input file */
    return error_count > 0; /* Return 0 on success (no errors), 1 on failure */
}

/* Function to check if a line is empty or a comment */
int is_empty_or_comment(const char *line) {
    const char *ptr = line;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Check if line is empty or starts with ';' */
    return (*ptr == '\0' || *ptr == ';');
}
