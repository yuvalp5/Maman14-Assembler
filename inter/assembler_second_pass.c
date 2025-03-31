/**
 * @brief Second pass of the assembler
 * Handles the generation of machine code and output files
 */

#include "assembler_second_pass.h"
#include "assembler_first_pass.h"
#include "definitions.h"
#include "types.h"
#include "utils.h"
#include <ctype.h>
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
        if (is_empty_or_comment_second_pass(line)) {
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
    char *fields[3] = {NULL, NULL, NULL};
    char *label = NULL;
    int current_ic = IC;
    int success = 1;
    int i;

    /* Skip leading whitespace */
    while (*line && (*line == ' ' || *line == '\t'))
        line++;

    /* Split line into fields */
    if (split_line_to_fields_second_pass(line, fields) != 0) {
        return 0;
    }

    /* First field is label if it ends with ':' */
    if (fields[0] && fields[0][strlen(fields[0]) - 1] == ':') {
        label = fields[0];
        label[strlen(label) - 1] = '\0'; /* Remove the ':' */
    }

    /* Second field is instruction/directive */
    if (!fields[1]) {
        return 0;
    }

    /* Handle entry declarations */
    if (strcmp(fields[1], "entry") == 0) {
        success = handle_entry_declaration(fields[2]);
    }
    /* Skip data and string directives - already handled in first pass */
    else if (strcmp(fields[1], "data") == 0 ||
             strcmp(fields[1], "string") == 0) {
        success = 1;
    }
    /* Skip external declarations - already handled in first pass */
    else if (strcmp(fields[1], "extern") == 0) {
        success = 1;
    }
    /* Process regular instruction */
    else {
        success = complete_instruction_encoding(fields[1], fields[2], memory,
                                                current_ic);
    }

    /* Free allocated fields */
    for (i = 0; i < 3; i++) {
        free(fields[i]);
    }

    return success;
}

int complete_instruction_encoding(char *instruction, char *operands,
                                  unsigned int *memory, int current_ic) {
    char *fields[2] = {NULL, NULL};
    Symbol *symbol;
    int addressing_mode;
    int success = 1;
    int i;

    /* Split operands if they exist */
    if (operands && split_line_to_fields_second_pass(operands, fields) != 0) {
        return 0;
    }

    /* Process first operand if exists */
    if (fields[0]) {
        addressing_mode = get_addressing_mode_second_pass(fields[0]);
        if (addressing_mode == DIRECT) {
            symbol = get_item(symbol_table, fields[0]);
            if (symbol) {
                if (symbol->symbol_type == SYMBOL_TYPE_EXTERN) {
                    memory[current_ic + 1] = 0; /* External symbol value is 0 */
                    update_externals_file(fields[0], current_ic + 1);
                } else {
                    memory[current_ic + 1] = symbol->symbol_value;
                }
            } else {
                printf("Error: Undefined symbol %s at line %d\n", fields[0],
                       line_number);
                success = 0;
            }
        }
    }

    /* Process second operand if exists */
    if (fields[1]) {
        addressing_mode = get_addressing_mode_second_pass(fields[1]);
        if (addressing_mode == DIRECT) {
            symbol = get_item(symbol_table, fields[1]);
            if (symbol) {
                if (symbol->symbol_type == SYMBOL_TYPE_EXTERN) {
                    memory[current_ic + 2] = 0; /* External symbol value is 0 */
                    update_externals_file(fields[1], current_ic + 2);
                } else {
                    memory[current_ic + 2] = symbol->symbol_value;
                }
            } else {
                printf("Error: Undefined symbol %s at line %d\n", fields[1],
                       line_number);
                success = 0;
            }
        }
    }

    /* Free allocated fields */
    for (i = 0; i < 2; i++) {
        free(fields[i]);
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

int is_empty_or_comment_second_pass(const char *line) {
    const char *ptr = line;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Check if line is empty or starts with ';' */
    return (*ptr == '\0' || *ptr == ';');
}

int split_line_to_fields_second_pass(char *line, char **fields) {
    char *ptr = line;
    char *field_start;
    int field_count = 0;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Process each field */
    while (*ptr && field_count < 3) {
        field_start = ptr;

        /* Skip until whitespace or end of line */
        while (*ptr && !isspace((unsigned char)*ptr) && *ptr != ';') {
            ptr++;
        }

        /* If we found a field */
        if (ptr > field_start) {
            fields[field_count] = malloc(ptr - field_start + 1);
            if (!fields[field_count]) {
                /* Clean up on allocation failure */
                while (field_count > 0) {
                    free(fields[--field_count]);
                }
                return -1;
            }
            strncpy(fields[field_count], field_start, ptr - field_start);
            fields[field_count][ptr - field_start] = '\0';
            field_count++;
        }

        /* Skip whitespace */
        while (*ptr && isspace((unsigned char)*ptr)) {
            ptr++;
        }
    }

    /* Initialize remaining fields to NULL */
    while (field_count < 3) {
        fields[field_count++] = NULL;
    }

    return 0;
}

AddressingMode get_addressing_mode_second_pass(const char *operand) {
    if (!operand) {
        return IMMEDIATE; /* Default mode */
    }

    /* Check for register direct addressing */
    if (strncmp(operand, "r", 1) == 0 && operand[1] >= '0' &&
        operand[1] <= '7') {
        return REGISTER_DIRECT;
    }

    /* Check for immediate addressing (#number) */
    if (operand[0] == '#') {
        return IMMEDIATE;
    }

    /* Check for direct addressing (label or number) */
    if (isalpha((unsigned char)operand[0]) || operand[0] == '_') {
        return DIRECT;
    }

    /* Check for relative addressing (label with offset) */
    if (operand[0] == '+' || operand[0] == '-') {
        return RELATIVE;
    }

    /* Default to direct addressing */
    return DIRECT;
}
