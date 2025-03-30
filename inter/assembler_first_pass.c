#include "inter/assembler_first_pass.h"
#include "definitions.h"
#include "types.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Memory for storing machine code - made global for second pass access */
int *code_memory = NULL;

/* Global variables from types.h */
extern int DC;              /* Data counter */
extern int IC;              /* Instruction counter */
extern int ICF;             /* Final IC value */
extern int DCF;             /* Final DC value */
extern int L;               /* Number of words in the instruction */
extern int symbol_count;    /* Number of symbols in the symbol table */
extern int error_count;     /* Number of errors */
extern int line_number;     /* Line number */
extern Table *symbol_table; /* Symbol table */

/* Helper function to handle symbol table operations */
int handle_symbol_addition(const char *symbol_name, int value, int type) {
    /* Check if symbol already exists in the table */
    if (find_symbol_in_table(symbol_name)) {
        printf("Error in line %d: Symbol '%s' already defined\n", line_number,
               symbol_name);
        error_count++;
        return 0;
    }

    /* Check if table is  full */
    if (symbol_table && symbol_table->count >= MAX_SYMBOLS) {
        printf("Error: Symbol table full, max %d symbols\n", MAX_SYMBOLS);
        error_count++;
        return 0;
    }

    /* Add symbol to table with specified type */
    if (add_symbol_to_table(symbol_name, value, type) != 0) {
        printf("Error in line %d: Failed to add symbol '%s' to table\n",
               line_number, symbol_name);
        error_count++;
        return 0;
    }

    return 1;
}

/* Helper function to initialize code memory if needed */
int initialize_code_memory() {
    int i;

    if (!code_memory) {
        code_memory = (int *)malloc(MAX_MEMORY_SIZE * sizeof(int));
        if (!code_memory) {
            printf("Error: Memory allocation failed for code storage\n");
            error_count++;
            return 0;
        }
        /* Initialize memory to zero */
        for (i = 0; i < MAX_MEMORY_SIZE; i++) {
            code_memory[i] = 0;
        }
    }
    return 1;
}

/* TODO: yuval-
get content from symol table:
((Macro *)macro_table->content[0])-><some_attr>)

search by name:
get_item(const Table *instance, const char *name);
*/

int first_pass(const char *src, const char *dest) {
    FILE *input_pre_assembled = NULL; /* File pointer for the input file */
    FILE *output_file = NULL;         /* File pointer for the output file */
    char line[MAX_LINE_LEN];          /* Line buffer */
    char line_copy[MAX_LINE_LEN];     /* Copy of the line to work with */
    int is_symbol_flag = 0;           /* Flag to check if a symbol is found */
    char *field;                      /* Field pointer */
    char *instruction;                /* Instruction pointer */
    char *operand1;                   /* Operand 1 pointer */
    char *operand2;                   /* Operand 2 pointer */
    unsigned int code[MAX_LINE_LEN];  /* To store generated machine code */
    int code_index = 0;               /* Index for the code array */
    int i;                            /* Counter for various loops */
    char symbol_name[MAX_SYMBOL_LEN + 1]; /* Symbol name buffer */
    int operand1_mode;                    /* Operand 1 mode */
    int operand2_mode;                    /* Operand 2 mode */
    int proceed_to_next_line =
        0; /* Flag to check if we should proceed to the next line */
    char *extern_symbol; /* External symbol pointer */
    char *operands;      /* For data storage operands */
    int len;             /* Length variable for string operations */

    /* Initialize IC and DC */
    IC = 100; /* Start at 100 as per requirements */
    DC = 0;   /* Data counter starts at 0 */

    /* Open files */
    input_pre_assembled = fopen(src, "r"); /* Open source file for reading */
    if (!input_pre_assembled) {
        perror("Error: Unable to open input file\n");
        return 1;
    }

    /* 2. Read the next line from the input file */
    while (fgets(line, MAX_LINE_LEN, input_pre_assembled) != NULL) {
        line_number++;
        is_symbol_flag = 0;
        code_index = 0;
        proceed_to_next_line = 0;

        /* Make a copy of the line to work with */
        strcpy(line_copy, line);

        /* 3. Check if first field is a symbol (label) */
        field = strtok(line_copy, " \t\n");

        if (field) {
            len = strlen(field);
            /* Check if field ends with a colon, indicating a symbol */
            if (len > 0 && field[len - 1] == ':') {
                /* Extract symbol name (without the colon) */
                strncpy(symbol_name, field, len - 1);
                symbol_name[len - 1] = '\0';

                if (!is_reserved_word(symbol_name)) {
                    is_symbol_flag = 1;
                    /* Get the next field (instruction) */
                    field = strtok(NULL, " \t\n");
                }
            }
        }

        /* Process the line only if it's not empty, not a comment, and we
         * have a field */
        if (field != NULL && field[0] != ';') {
            /* 5. Check if line contains a data storage directive (.data or
             * .string) */
            if (is_storage(field)) {
                /* 6. If symbol definition found, add the symbol to the
                 * symbol table */
                if (is_symbol_flag) {
                    handle_symbol_addition(symbol_name, DC, SYMBOL_TYPE_DATA);
                }

                /* 7. Encode the data values in memory, update DC */
                operands = strtok(NULL, " \t\n");
                if (!encode_data_storage(field, operands, code_memory)) {
                    proceed_to_next_line = 1;
                    continue;
                }
                proceed_to_next_line = 1;
            }
            /* 8. Check if line contains .extern or .entry directive */
            else if (is_extern_or_entry(field)) {
                /* 9. If directive is .entry, skip it (will be handled in
                 * second pass) */
                if (strcmp(field, ".entry") != 0) {
                    /* 10. If directive is .extern, add the symbol to the
                     * symbol table */
                    if (strcmp(field, ".extern") == 0) {
                        /* Get the operand of .extern */
                        extern_symbol = strtok(NULL, " \t\n");

                        if (extern_symbol) {
                            handle_symbol_addition(extern_symbol, 0,
                                                   SYMBOL_TYPE_EXTERN);
                        } else { /* if the operand is missing, print an
                                    error message */
                            printf("Error in line %d: Missing operand for "
                                   ".extern directive\n",
                                   line_number);
                            error_count++;
                        }
                    }
                }
                proceed_to_next_line = 1;
            }
            /* This is an instruction line */
            else if (!proceed_to_next_line) {
                /* 11. If symbol definition found, add to symbol table */
                if (is_symbol_flag) {
                    handle_symbol_addition(symbol_name, IC, SYMBOL_TYPE_CODE);
                }

                /* Process instruction */
                instruction = field;
                if (search_command_in_table(instruction)) {
                    /* Get operands and calculate word count */
                    operand1 = strtok(NULL, " \t\n,");
                    operand2 = strtok(NULL, " \t\n,");

                    /* Get addressing modes */
                    operand1_mode =
                        operand1 ? get_addressing_mode(operand1) : -1;
                    operand2_mode =
                        operand2 ? get_addressing_mode(operand2) : -1;

                    /* Validate operands */
                    if (!validate_operand(operand1,
                                          search_command_in_table(instruction),
                                          1, operand1_mode) ||
                        !validate_operand(operand2,
                                          search_command_in_table(instruction),
                                          2, operand2_mode)) {
                        proceed_to_next_line = 1;
                        continue;
                    }

                    /* Calculate the total word count (L) for this
                     * instruction */
                    L = calc_num_of_words(instruction, operand1, operand2);

                    /* Generate binary code for the instruction */
                    word_to_binary(search_command_in_table(instruction),
                                   operand1_mode, operand2_mode, code,
                                   &code_index);

                    /* Add operand values to the code array */
                    if (operand1_mode >= 0) {
                        switch (operand1_mode) {
                        case 0: /* Immediate */
                            code[code_index++] = atoi(operand1 + 1);
                            break;
                        case 1: /* Direct */
                            code[code_index++] =
                                0; /* Will be updated in second pass */
                            break;
                        case 2: /* Index */
                            code[code_index++] =
                                0; /* Will be updated in second pass */
                            code[code_index++] =
                                operand1[strlen(operand1) - 2] - '0';
                            break;
                        }
                    }

                    if (operand2_mode >= 0) {
                        switch (operand2_mode) {
                        case 0: /* Immediate */
                            code[code_index++] = atoi(operand2 + 1);
                            break;
                        case 1: /* Direct */
                            code[code_index++] =
                                0; /* Will be updated in second pass */
                            break;
                        case 2: /* Index */
                            code[code_index++] =
                                0; /* Will be updated in second pass */
                            code[code_index++] =
                                operand2[strlen(operand2) - 2] - '0';
                            break;
                        }
                    }

                    /* Save current IC and L values along with the machine
                     * code generated */
                    save_values_with_binary(code, code_index, NULL, IC);

                    /* Update IC */
                    IC += L;
                } else {
                    printf("Error in line %d: Unknown instruction '%s'\n",
                           line_number, instruction);
                    error_count++;
                }
            }
        }
    }

    /* 17. End of first pass, save final values of IC and DC as ICF and DCF
     */
    ICF = IC;
    DCF = DC;

    /* 18. Update data symbols in the symbol table by adding ICF to their
     * values */
    add_lcf_to_data(ICF);

    /* Clean up input file handle */
    fclose(input_pre_assembled);

    /* Note: code_memory is preserved for use in the second pass */

    /* Print summary */
    printf("First pass finished.\n");
    printf("ICF = %d, DCF = %d\n", ICF, DCF);
    printf("Symbol count = %zu\n", symbol_table ? symbol_table->count : 0);
    printf("Error count = %d\n", error_count);

    /* Print symbol table */
    print_symbol_table();

    if (error_count > 0) {
        printf("First pass completed with errors.\n");
    } else {
        printf("First pass completed successfully.\n");
    }
    printf("Total errors: %d\n", error_count);

    /* Write first pass output to file if no errors */
    if (error_count == 0) {
        /* Now open the output file for writing since we know there are no
         * errors */
        output_file = fopen(dest, "w");
        if (!output_file) {
            perror("Error: Unable to open output file for writing results\n");
            return 1;
        }

        /* Write header with ICF and DCF */
        fprintf(output_file, "ICF=%d\tDCF=%d\n", ICF, DCF);

        /* Write code and data segments to the output file */
        fprintf(output_file, "Code segment:\n");
        for (i = 100; i < ICF; i++) {
            if (code_memory && i < MAX_MEMORY_SIZE) {
                fprintf(output_file, "%04d\t%06X\n", i, code_memory[i]);
            }
        }

        fprintf(output_file, "Data segment:\n");
        for (i = 0; i < DCF; i++) {
            if (code_memory && ICF + i < MAX_MEMORY_SIZE) {
                fprintf(output_file, "%04d\t%06X\n", ICF + i,
                        code_memory[ICF + i]);
            }
        }

        printf("First pass output written to %s\n", dest);

        /* Close the output file */
        fclose(output_file);
    }

    return 0;
}

/* Helper functions */

int is_symbol(char *field) {
    int len;
    char symbol_name[MAX_SYMBOL_LEN + 1];

    /* Check if the field ends with a colon, indicating a label/symbol */
    len = strlen(field);

    /* Check if the field is a valid symbol name */
    if (len > 0 && field[len - 1] == ':') {
        /* Create a temporary copy of the symbol name without the colon */
        strncpy(symbol_name, field, len - 1);
        symbol_name[len - 1] = '\0';

        /* Check if the symbol name is valid (not empty and not a reserved word)
         */
        if (strlen(symbol_name) > 0 && !is_reserved_word(symbol_name)) {
            return 1;
        }
    }

    return 0;
}

int is_reserved_word(const char *word) {
    /* List of reserved words to check against */
    const char *reserved_words[] = {
        "mov",   "cmp",     "add",    "sub",     "lea", "clr", "not", "inc",
        "dec",   "jmp",     "bne",    "jsr",     "red", "prn", "rts", "stop",
        ".data", ".string", ".entry", ".extern", "r0",  "r1",  "r2",  "r3",
        "r4",    "r5",      "r6",     "r7",      NULL};
    int i = 0;

    while (reserved_words[i] != NULL) {
        if (strcmp(word, reserved_words[i]) == 0) {
            return 1; /* It is a reserved word */
        }
        i++;
    }

    return 0; /* Not a reserved word */
}

int is_storage(char *field) {
    return (strcmp(field, ".data") == 0 || strcmp(field, ".string") == 0);
}

int is_extern_or_entry(char *field) {
    return (strcmp(field, ".extern") == 0 || strcmp(field, ".entry") == 0);
}

int search_command_in_table(char *command) {
    /* List of valid commands and their indices */
    const char *commands[] = {"mov", "cmp", "add", "sub",  "lea", "clr",
                              "not", "inc", "dec", "jmp",  "bne", "jsr",
                              "red", "prn", "rts", "stop", NULL};

    int i = 0;
    while (commands[i] != NULL) {
        if (strcmp(command, commands[i]) == 0) {
            return i +
                   1; /* Return index + 1 to avoid returning 0 for a match */
        }
        i++;
    }

    return 0; /* Command not found */
}

int calc_num_of_words(char *instruction, char *operand1, char *operand2) {
    int word_count = 1; /* Base word count is 1 for the instruction itself */
    int instr_idx = search_command_in_table(instruction);

    /* Calculate words based on instruction type and operands */
    if (instr_idx >= 1 && instr_idx <= 5) {
        /* Two-operand instructions: mov, cmp, add, sub, lea */
        if (operand1) {
            int mode1 = get_addressing_mode(operand1);
            word_count += get_operand_words(mode1);
        }

        if (operand2) {
            int mode2 = get_addressing_mode(operand2);
            word_count += get_operand_words(mode2);
        }
    } else if (instr_idx >= 6 && instr_idx <= 14) {
        /* Single-operand instructions: clr, not, inc, dec, jmp, bne, jsr, red,
         * prn */
        if (operand1) {
            int mode = get_addressing_mode(operand1);
            word_count += get_operand_words(mode);
        }
    }
    /* No-operand instructions (rts, stop) don't need extra words */

    return word_count;
}

int get_addressing_mode(char *operand) {
    /* Determine addressing mode based on operand format */
    if (!operand) {
        return -1; /* No operand */
    }

    /* Check for immediate addressing (#value) */
    if (operand[0] == '#') {
        return 0;
    }

    /* Check for register direct addressing (r0-r7) */
    if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7' &&
        operand[2] == '\0') {
        return 3;
    }

    /* Check for index addressing (label[rx]) */
    if (strchr(operand, '[') != NULL) {
        return 2;
    }

    /* Check for jump with & prefix (for jmp, bne, jsr) */
    if (operand[0] == '&') {
        return 1; /* Treat as direct addressing */
    }

    /* Default to direct addressing */
    return 1;
}

int get_operand_words(int addressing_mode) {
    int result = 0;

    switch (addressing_mode) {
    case 0: /* Immediate */
        result = 1;
        break;
    case 1: /* Direct */
        result = 1;
        break;
    case 2: /* Index */
        result = 2;
        break;
    case 3:         /* Register Direct */
        result = 0; /* Register operands don't need extra words */
        break;
    default:
        result = 0;
        break;
    }

    return result;
}

void word_to_binary(int instruction_index, int operand1_mode, int operand2_mode,
                    unsigned int *code, int *code_index) {
    /* First word: op-code (4 bits), addressing modes (4 bits), function code (4
     * bits) */
    unsigned int first_word = 0;

    /* Set opcode (4 bits) */
    first_word |= (instruction_index & 0xF) << 8;

    /* Set addressing modes (2 bits each) */
    if (operand1_mode >= 0) {
        first_word |= (operand1_mode & 0x3) << 4;
    }
    if (operand2_mode >= 0) {
        first_word |= (operand2_mode & 0x3) << 2;
    }

    /* Set function code for arithmetic operations */
    switch (instruction_index) {
    case 3:                /* add */
        first_word |= 0x1; /* Function code 1 for add */
        break;
    case 4:                /* sub */
        first_word |= 0x2; /* Function code 2 for sub */
        break;
    case 5:                /* lea */
        first_word |= 0x3; /* Function code 3 for lea */
        break;
    default:
        first_word |= 0x0; /* No function code needed */
        break;
    }

    /* Store the first word */
    code[(*code_index)++] = first_word;

    /* Add operand words based on addressing modes */
    if (operand1_mode >= 0) {
        switch (operand1_mode) {
        case 0: /* Immediate */
            /* Value will be added by the caller */
            break;
        case 1: /* Direct */
            /* Label address will be added by the caller */
            break;
        case 2: /* Index */
            /* Label address and register will be added by the caller */
            break;
        case 3: /* Register */
            /* Register number is encoded in the first word */
            break;
        }
    }

    if (operand2_mode >= 0) {
        switch (operand2_mode) {
        case 0: /* Immediate */
            /* Value will be added by the caller */
            break;
        case 1: /* Direct */
            /* Label address will be added by the caller */
            break;
        case 2: /* Index */
            /* Label address and register will be added by the caller */
            break;
        case 3: /* Register */
            /* Register number is encoded in the first word */
            break;
        }
    }
}

void save_values_with_binary(unsigned int *code, int word_count, int *memory,
                             int current_ic) {
    /* Store the generated machine code in memory */
    int i;

    /* Initialize code memory if needed */
    if (!initialize_code_memory()) {
        return;
    }

    /* Store each word of machine code */
    for (i = 0; i < word_count; i++) {
        /* Check if we're within memory bounds */
        if (current_ic + i >= MAX_MEMORY_SIZE) {
            printf("Error: Memory overflow at address %d\n", current_ic + i);
            return;
        }

        /* Store the word */
        code_memory[current_ic + i] = code[i];

        /* Debug output */
        printf("Memory[%d] = 0x%04X\n", current_ic + i, code[i]);
    }
}

void add_lcf_to_data(int offset) {
    int i;
    Symbol *symbol;

    printf("Adding offset %d to data symbols\n", offset);

    /* Iterate through the symbol table using the Table structure */
    if (symbol_table && symbol_table->content) {
        for (i = 0; i < symbol_table->count; i++) {
            symbol = (Symbol *)symbol_table->content[i];
            if (symbol && symbol->symbol_type == SYMBOL_TYPE_DATA) {
                /* Add the full ICF value to each data symbol */
                symbol->symbol_value += offset;
                printf("Updated symbol '%s' to value %d\n", symbol->name,
                       symbol->symbol_value);
            }
        }
    }
}

/* Validate operand based on instruction and addressing mode */
int validate_operand(char *operand, int instruction_index, int operand_num,
                     int addressing_mode) {
    char *value;   /* For immediate value validation */
    char *bracket; /* For index addressing validation */
    char *reg;     /* For register validation */
    int reg_num;   /* For register number validation */

    /* Handle no-operand instructions first */
    if (instruction_index == 15 || instruction_index == 16) { /* rts or stop */
        if (operand != NULL && operand_num == 1) {
            printf("Error in line %d: Unexpected operand for no-operand "
                   "instruction\n",
                   line_number);
            error_count++;
            return 0;
        }
        return 1;
    }

    /* Handle single-operand instructions */
    if (instruction_index >= 6 && instruction_index <= 14) { /* clr to prn */
        if (operand_num == 2) {
            /* These instructions don't have a second operand */
            return 1;
        }
    }

    /* Check for NULL operand for instructions that require operands */
    if (!operand) {
        /* For single-operand instructions, only operand1 is required */
        if (instruction_index >= 6 &&
            instruction_index <= 14) { /* clr to prn */
            if (operand_num == 1) {
                printf("Error in line %d: Missing operand for instruction\n",
                       line_number);
                error_count++;
                return 0;
            }
        } else if (instruction_index >= 1 &&
                   instruction_index <= 5) { /* Two-operand instructions */
            printf("Error in line %d: Missing operand %d for instruction\n",
                   line_number, operand_num);
            error_count++;
            return 0;
        }
        return 1;
    }

    /* Handle jump instructions with & prefix */
    if ((instruction_index == 10 || instruction_index == 11 ||
         instruction_index == 12) && /* jmp, bne, jsr */
        operand[0] == '&') {
        /* Remove the & for validation */
        operand++;
        addressing_mode = 1; /* Direct addressing */
    }

    /* Validate addressing mode based on instruction */
    switch (instruction_index) {
    case 1:                     /* mov */
        if (operand_num == 1) { /* Source operand */
            if (addressing_mode == -1) {
                printf("Error in line %d: Invalid source operand for mov\n",
                       line_number);
                error_count++;
                return 0;
            }
        } else { /* Destination operand */
            if (addressing_mode == 0 || addressing_mode == -1) {
                printf(
                    "Error in line %d: Invalid destination operand for mov\n",
                    line_number);
                error_count++;
                return 0;
            }
        }
        break;

    case 2: /* cmp */
        if (addressing_mode == -1) {
            printf("Error in line %d: Invalid operand for cmp\n", line_number);
            error_count++;
            return 0;
        }
        break;

    case 3:                     /* add */
    case 4:                     /* sub */
        if (operand_num == 1) { /* Source operand */
            if (addressing_mode == -1) {
                printf("Error in line %d: Invalid source operand for "
                       "arithmetic operation\n",
                       line_number);
                error_count++;
                return 0;
            }
        } else { /* Destination operand */
            if (addressing_mode == 0 || addressing_mode == -1) {
                printf("Error in line %d: Invalid destination operand for "
                       "arithmetic operation\n",
                       line_number);
                error_count++;
                return 0;
            }
        }
        break;

    case 5:                             /* lea */
        if (operand_num == 1) {         /* Source operand */
            if (addressing_mode != 1) { /* Must be direct addressing */
                printf("Error in line %d: lea requires direct addressing for "
                       "source\n",
                       line_number);
                error_count++;
                return 0;
            }
        } else { /* Destination operand */
            if (addressing_mode == 0 || addressing_mode == -1) {
                printf(
                    "Error in line %d: Invalid destination operand for lea\n",
                    line_number);
                error_count++;
                return 0;
            }
        }
        break;

    case 6: /* clr */
    case 7: /* not */
    case 8: /* inc */
    case 9: /* dec */
        if (addressing_mode == 0 || addressing_mode == -1) {
            printf("Error in line %d: Invalid operand for single-operand "
                   "instruction\n",
                   line_number);
            error_count++;
            return 0;
        }
        break;

    case 10: /* jmp */
    case 11: /* bne */
    case 12: /* jsr */
        if (addressing_mode == 0 || addressing_mode == -1) {
            printf("Error in line %d: Invalid operand for jump instruction\n",
                   line_number);
            error_count++;
            return 0;
        }
        break;

    case 13: /* red */
        if (addressing_mode == 0 || addressing_mode == -1) {
            printf("Error in line %d: Invalid operand for red instruction\n",
                   line_number);
            error_count++;
            return 0;
        }
        break;

    case 14: /* prn */
        if (addressing_mode == -1) {
            printf("Error in line %d: Invalid operand for prn instruction\n",
                   line_number);
            error_count++;
            return 0;
        }
        break;
    }

    /* Validate immediate values */
    if (addressing_mode == 0) { /* Immediate addressing */
        value = operand + 1;    /* Skip the '#' */
        if (!isdigit(value[0]) && value[0] != '-') {
            printf("Error in line %d: Invalid immediate value '%s'\n",
                   line_number, operand);
            error_count++;
            return 0;
        }
    }

    /* Validate register numbers */
    if (addressing_mode == 3) { /* Register addressing */
        reg = operand + 1;
        reg_num = reg[0] - '0';
        if (reg_num < 0 || reg_num > 7) {
            printf("Error in line %d: Invalid register number '%s'\n",
                   line_number, operand);
            error_count++;
            return 0;
        }
    }

    /* Validate index addressing */
    if (addressing_mode == 2) { /* Index addressing */
        bracket = strchr(operand, '[');
        if (!bracket) {
            printf("Error in line %d: Invalid index addressing format '%s'\n",
                   line_number, operand);
            error_count++;
            return 0;
        }

        /* Check register part */
        reg = bracket + 1;
        if (reg[0] != 'r' || reg[1] < '0' || reg[1] > '7' || reg[2] != ']') {
            printf(
                "Error in line %d: Invalid register in index addressing '%s'\n",
                line_number, operand);
            error_count++;
            return 0;
        }
    }

    return 1;
}

/* Encode data storage directive */
int encode_data_storage(const char *directive, char *operands,
                        int *data_memory) {
    char *token;
    int value;
    int i;
    char operands_copy[MAX_LINE_LEN];

    /* Initialize code memory if needed */
    if (!initialize_code_memory()) {
        return 0;
    }
    data_memory = code_memory;

    if (strcmp(directive, ".data") == 0) {
        /* Handle .data directive */
        if (!operands) {
            printf("Error in line %d: Missing operands for .data directive\n",
                   line_number);
            error_count++;
            return 0;
        }

        /* Make a copy of operands to tokenize */
        strcpy(operands_copy, operands);
        token = strtok(operands_copy, ",");

        while (token != NULL) {
            /* Skip leading whitespace */
            while (*token == ' ' || *token == '\t')
                token++;

            /* Check if it's a valid number */
            if (!isdigit(token[0]) && token[0] != '-') {
                printf("Error in line %d: Invalid number in .data directive: "
                       "'%s'\n",
                       line_number, token);
                error_count++;
                return 0;
            }

            /* Convert string to number */
            value = atoi(token);

            /* Store the value in data memory */
            if (DC >= MAX_MEMORY_SIZE) {
                printf("Error in line %d: Data segment overflow\n",
                       line_number);
                error_count++;
                return 0;
            }

            code_memory[DC++] = value;
            printf("Data[%d] = %d\n", DC - 1, value); /* Debug output */

            /* Get next number */
            token = strtok(NULL, ",");
        }
    } else if (strcmp(directive, ".string") == 0) {
        /* Handle .string directive */
        if (!operands) {
            printf("Error in line %d: Missing string for .string directive\n",
                   line_number);
            error_count++;
            return 0;
        }

        /* Remove quotes if present */
        if (operands[0] == '"') {
            operands++;
            if (operands[strlen(operands) - 1] == '"') {
                operands[strlen(operands) - 1] = '\0';
            }
        }

        /* Store each character plus null terminator */
        for (i = 0; operands[i] != '\0'; i++) {
            if (DC >= MAX_MEMORY_SIZE) {
                printf("Error in line %d: Data segment overflow\n",
                       line_number);
                error_count++;
                return 0;
            }
            code_memory[DC++] = operands[i];
            printf("Data[%d] = %d ('%c')\n", DC - 1, operands[i],
                   operands[i]); /* Debug output */
        }

        /* Add null terminator */
        if (DC >= MAX_MEMORY_SIZE) {
            printf("Error in line %d: Data segment overflow\n", line_number);
            error_count++;
            return 0;
        }
        code_memory[DC++] = '\0';
        printf("Data[%d] = 0 ('\\0')\n", DC - 1); /* Debug output */
    }

    return 1;
}

/* Print the symbol table contents using the Table structure for debugging */
void print_symbol_table(void) {
    int i;
    Symbol *symbol;

    printf("\nSymbol Table Contents:\n");
    printf("----------------------\n");
    printf("Name\t\tValue\tType\n");
    printf("----------------------\n");

    if (symbol_table && symbol_table->content) {
        for (i = 0; i < symbol_table->count; i++) {
            symbol = (Symbol *)symbol_table->content[i];
            if (symbol) {
                printf("%-16s %-8d ", symbol->name, symbol->symbol_value);
                switch (symbol->symbol_type) {
                case SYMBOL_TYPE_CODE:
                    printf("CODE\n");
                    break;
                case SYMBOL_TYPE_DATA:
                    printf("DATA\n");
                    break;
                case SYMBOL_TYPE_EXTERN:
                    printf("EXTERN\n");
                    break;
                case SYMBOL_TYPE_ENTRY:
                    printf("ENTRY\n");
                    break;
                default:
                    printf("UNKNOWN\n");
                    break;
                }
            }
        }
    }
    printf("----------------------\n");
}

/* Implementation of symbol table functions using the proper API */
int add_symbol_to_table(const char *name, int value, int type) {
    int result = insert_symbol(name, value, type);

    /* Note: The Table structure already updates its count internally
       through the insert_symbol function, which calls _insert_item.
       We still update symbol_count for backward compatibility with
       other parts of the code that might rely on it. */
    if (result == 0 && symbol_table) {
        symbol_count = symbol_table->count;
    }

    return result;
}

int find_symbol_in_table(const char *name) {
    Symbol *symbol = (Symbol *)get_item(symbol_table, name);
    return (symbol != NULL);
}

/* Modified symbol table search function */
int get_symbol_value(const char *name) {
    Symbol *symbol = (Symbol *)get_item(symbol_table, name);
    if (symbol) {
        return symbol->symbol_value;
    }
    return -1; /* Symbol not found */
}

int get_symbol_type(const char *name) {
    Symbol *symbol = (Symbol *)get_item(symbol_table, name);
    if (symbol) {
        return symbol->symbol_type;
    }
    return -1; /* Symbol not found */
}
