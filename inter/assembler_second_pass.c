/**
 * @brief Second pass of the assembler
 * Handles the generation of machine code and output files
 */

#include "assembler_second_pass.h"
#include "assembler_first_pass.h"
#include "definitions.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* File handles for output files */
static FILE *externals_file = NULL;
static FILE *entries_file = NULL;
static FILE *object_file = NULL;

/* Memory array for machine code */
static unsigned int memory[MAX_MEMORY_SIZE];

int second_pass(const char *src_file, const char *dest_file) {
    FILE *fp;
    char line[MAX_LINE_LEN];
    char base_name[MAX_LINE_LEN];
    int success = 1;

    /* Reset line number counter */
    line_number = 1;

    /* Extract base name for output files */
    strncpy(base_name, dest_file, strlen(dest_file) - strlen(OBJ_F_EXT));
    base_name[strlen(dest_file) - strlen(OBJ_F_EXT)] = '\0';

    /* Open source file */
    fp = fopen(src_file, "r");
    if (!fp) {
        printf("Error: Could not open source file %s\n", src_file);
        return 0;
    }

    /* Process each line */
    while (fgets(line, MAX_LINE_LEN, fp)) {
        /* Skip empty lines and comments */
        if (is_empty_or_comment(line)) {
            line_number++;
            continue;
        }

        /* Process the line */
        if (!process_line_second_pass(line, memory)) {
            success = 0;
        }
        line_number++;
    }

    fclose(fp);

    /* If no errors, generate output files */
    if (success && error_count == 0) {
        success = generate_output_files(base_name, memory);
    }

    return success;
}

int process_line_second_pass(char *line, unsigned int *memory) {
    char *token, *instruction, *operands;
    char *label = NULL;
    int current_ic = IC;

    /* Skip leading whitespace */
    while (*line && (*line == ' ' || *line == '\t'))
        line++;

    /* Check for label */
    token = split_line_to_fields(line, 1, ":");
    if (token && token[strlen(token) - 1] == ':') {
        label = token;
        line += strlen(label) + 1;
        while (*line && (*line == ' ' || *line == '\t'))
            line++;
    }

    /* Get instruction/directive */
    instruction = split_line_to_fields(line, 1, " \t");
    if (!instruction)
        return 1;

    /* Handle entry declarations */
    if (strcmp(instruction, "entry") == 0) {
        operands = split_line_to_fields(line, 2, " \t");
        return handle_entry_declaration(operands);
    }

    /* Skip data and string directives - already handled in first pass */
    if (strcmp(instruction, "data") == 0 ||
        strcmp(instruction, "string") == 0) {
        return 1;
    }

    /* Skip external declarations - already handled in first pass */
    if (strcmp(instruction, "extern") == 0) {
        return 1;
    }

    /* Get operands */
    operands = strchr(line, ' ');
    if (operands) {
        operands++;
        while (*operands && (*operands == ' ' || *operands == '\t'))
            operands++;
    }

    /* Complete instruction encoding */
    return complete_instruction_encoding(instruction, operands, memory,
                                         current_ic);
}

int complete_instruction_encoding(char *instruction, char *operands,
                                  unsigned int *memory, int current_ic) {
    char *operand1 = NULL, *operand2 = NULL;
    Symbol *symbol;
    int addressing_mode;
    int success = 1;

    /* Split operands */
    if (operands) {
        operand1 = split_line_to_fields(operands, 1, ",");
        operand2 = split_line_to_fields(operands, 2, ",");
    }

    /* Process first operand if exists */
    if (operand1) {
        addressing_mode = get_addressing_mode(operand1);
        if (addressing_mode == ADDR_MODE_DIRECT) {
            symbol = get_item(symbol_table, operand1);
            if (symbol) {
                if (symbol->symbol_type == SYMBOL_TYPE_EXTERN) {
                    memory[current_ic + 1] = 0; /* External symbol value is 0 */
                    update_externals_file(operand1, current_ic + 1);
                } else {
                    memory[current_ic + 1] = symbol->symbol_value;
                }
            } else {
                printf("Error: Undefined symbol %s at line %d\n", operand1,
                       line_number);
                success = 0;
            }
        }
    }

    /* Process second operand if exists */
    if (operand2) {
        addressing_mode = get_addressing_mode(operand2);
        if (addressing_mode == ADDR_MODE_DIRECT) {
            symbol = get_item(symbol_table, operand2);
            if (symbol) {
                if (symbol->symbol_type == SYMBOL_TYPE_EXTERN) {
                    memory[current_ic + 2] = 0; /* External symbol value is 0 */
                    update_externals_file(operand2, current_ic + 2);
                } else {
                    memory[current_ic + 2] = symbol->symbol_value;
                }
            } else {
                printf("Error: Undefined symbol %s at line %d\n", operand2,
                       line_number);
                success = 0;
            }
        }
    }

    return success;
}

int generate_output_files(const char *base_name, unsigned int *memory) {
    char obj_name[MAX_LINE_LEN];
    char ext_name[MAX_LINE_LEN];
    char ent_name[MAX_LINE_LEN];
    int i;

    /* Create file names */
    sprintf(obj_name, "%s%s", base_name, OBJ_F_EXT);
    sprintf(ext_name, "%s%s", base_name, EXT_F_EXT);
    sprintf(ent_name, "%s%s", base_name, ENT_F_EXT);

    /* Open object file */
    object_file = fopen(obj_name, "w");
    if (!object_file) {
        printf("Error: Could not create object file %s\n", obj_name);
        return 0;
    }

    /* Write header with instruction and data counts */
    fprintf(object_file, "%d %d\n", ICF - 100, DCF);

    /* Write memory image */
    for (i = 100; i < ICF + DCF; i++) {
        fprintf(object_file, "%07d %06x\n", i, memory[i]);
    }

    fclose(object_file);
    return 1;
}

int handle_entry_declaration(char *operand) {
    Symbol *symbol;
    char ent_name[MAX_LINE_LEN];

    if (!operand) {
        printf("Error: Missing operand for entry directive at line %d\n",
               line_number);
        return 0;
    }

    /* Find symbol in symbol table */
    symbol = get_item(symbol_table, operand);
    if (!symbol) {
        printf("Error: Undefined symbol %s in entry directive at line %d\n",
               operand, line_number);
        return 0;
    }

    /* Open entries file if not already open */
    if (!entries_file) {
        sprintf(ent_name, "%s%s", operand, ENT_F_EXT);
        entries_file = fopen(ent_name, "w");
        if (!entries_file) {
            printf("Error: Could not create entries file\n");
            return 0;
        }
    }

    /* Write entry to file */
    fprintf(entries_file, "%s %07d\n", operand, symbol->symbol_value);

    /* Update symbol type to include entry */
    symbol->symbol_type |= SYMBOL_TYPE_ENTRY;

    return 1;
}

int update_externals_file(const char *symbol, int address) {
    char ext_name[MAX_LINE_LEN];

    /* Open externals file if not already open */
    if (!externals_file) {
        sprintf(ext_name, "%s%s", symbol, EXT_F_EXT);
        externals_file = fopen(ext_name, "w");
        if (!externals_file) {
            printf("Error: Could not create externals file\n");
            return 0;
        }
    }

    /* Write external reference to file */
    fprintf(externals_file, "%s %07d\n", symbol, address);
    return 1;
}
