#include "../inter/assembler_first_pass.h"

/* Global variables */
int DC = 0;           /* Data counter initialized to 0 */
int IC = 100;         /* Instruction counter initialized to 100 */
int symbol_count = 0; /* Number of symbols in the symbol table */
int error_count = 0;  /* Number of errors */
int line_number = 0;  /* Line number */
int ICF, DCF;         /* Final IC and DC values */
int L;                /* Number of words in the instruction */

/* Symbol table */
Symbol symbol_table[MAX_SYMBOLS];

int first_pass(const char *src) {
    FILE *input_pre_assembled = NULL; /* File pointer for the input file */
    char line[MAX_LINE_LEN];          /* Line buffer */
    char line_copy[MAX_LINE_LEN];     /* Copy of the line to work with */
    int is_symbol_flag = 0;           /* Flag to check if a symbol is found */
    char *field;                      /* Field pointer */
    char *instruction;                /* Instruction pointer */
    char *operand1;                   /* Operand 1 pointer */
    char *operand2;                   /* Operand 2 pointer */
    unsigned int code[MAX_LINE_LEN];  /* To store generated machine code */
    int code_index = 0;               /* Index for the code array */
    int i; /* C90 requires declarations at the beginning of blocks */
    char symbol_name[MAX_SYMBOL_LEN + 1]; /* Symbol name buffer */
    int operand1_mode;                    /* Operand 1 mode */
    int operand2_mode;                    /* Operand 2 mode */
    int proceed_to_next_line; /* Flag to check if we should proceed to the next
                                 line */
    char *extern_symbol;
    int symbol_found; /* Flag to track if a symbol was found in the loop */

    /* 1. Initialize IC and DC (already done globally) */

    /* Open .am source file for reading */
    input_pre_assembled = fopen(src, "r");
    if (!input_pre_assembled) {
        perror(
            "Error: Unable to open input file\n"); /* Print error message if
                                                      file cannot be opened */
    } else {
        /* 2. Read the next line from the input file */
        while (fgets(line, MAX_LINE_LEN, input_pre_assembled) != NULL) {
            line_number++;
            is_symbol_flag = 0;       /* Reset the flag */
            code_index = 0;           /* Reset the code index */
            proceed_to_next_line = 0; /* Reset the flag */

            /* Make a copy of the line to work with */
            strcpy(line_copy, line);

            /* 3. Check if first field is a symbol (label) */
            field = split_line_to_fields(
                line_copy, 0, " \t\n:"); /* Split the line into fields */
            if (field &&
                is_symbol(field)) { /* Check if the field is a symbol */
                /* 4. Set a flag indicating symbol definition was found */
                is_symbol_flag = 1; /* Set the flag */

                /* Get the next field after the symbol */
                field = split_line_to_fields(NULL, 0, " \t\n");
            }

            /* Process the line only if it's not empty and not a comment */
            if (field != NULL) {
                /* 5. Check if line contains a data storage directive (.data or
                 * .string) */
                if (is_storage(field)) {
                    /* 6. If symbol definition found, add the symbol to the
                     * symbol table */
                    if (is_symbol_flag) {
                        /* Get the symbol name from the original line */
                        strcpy(symbol_name,
                               split_line_to_fields(line, 0, " \t\n:"));

                        /* Add symbol to table with type DATA */
                        if (symbol_count < MAX_SYMBOLS) {
                            /* Check if symbol already exists in the table */
                            symbol_found = 0;
                            for (i = 0; i < symbol_count && !symbol_found;
                                 i++) {
                                if (strcmp(symbol_table[i].symbol_name,
                                           symbol_name) == 0) {
                                    printf("Error in line %d: Symbol '%s' "
                                           "already defined\n",
                                           line_number, symbol_name);
                                    error_count++;
                                    symbol_found = 1;
                                }
                            }

                            if (!symbol_found) {
                                strcpy(symbol_table[symbol_count].symbol_name, symbol_name);
                                symbol_table[symbol_count].symbol_value = DC;
                                symbol_table[symbol_count].symbol_type = SYMBOL_TYPE_DATA;
                                symbol_count++;
                            }
                        } else {
                            printf("Error: Symbol table full, max %d symbols\n", MAX_SYMBOLS);
                            error_count++;
                        }
                    }

                    /* 7. Encode the data values in memory, update DC and go to
                     * step 2 */
                    /* TODO: Implement encoding of data values */
                    /* For now, we'll just increment DC by a placeholder amount
                     */
                    DC += 4; /* Placeholder, replace with actual data encoding
                                logic */
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
                            extern_symbol =
                                split_line_to_fields(NULL, 0, " \t\n");

                            if (extern_symbol) {
                                if (symbol_count < MAX_SYMBOLS) {
                                    /* Check if symbol already exists in the
                                     * table */
                                    symbol_found = 0; /* Reset the flag */
                                    for (i = 0; i < symbol_count && !symbol_found; i++) { /* Loop through the symbol table */
                                        if (strcmp(symbol_table[i].symbol_name, extern_symbol) == 0) { /* Check if the symbol already exists */
                                            printf("Error in line %d: Symbol "  "'%s' already defined\n", line_number, extern_symbol);
                                            error_count++;
                                            symbol_found = 1; /* Set the flag */
                                        }
                                    }

                                    if (!symbol_found) {
                                        strcpy(symbol_table[symbol_count].symbol_name, extern_symbol);
                                        symbol_table[symbol_count].symbol_value = 0;
                                        symbol_table[symbol_count].symbol_type = SYMBOL_TYPE_EXTERN;
                                        symbol_count++;
                                    }
                                } else {
                                    printf("Error: Symbol table full, max %d " "symbols\n", MAX_SYMBOLS);
                                    error_count++;
                                }
                            } else {
                                printf("Error in line %d: Missing operand for " "directive\n", line_number);
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
                        /* Get the symbol name from the original line */
                        strcpy(symbol_name,
                               split_line_to_fields(line, 0, " \t\n:"));

                        /* Add symbol to table with type CODE */
                        if (symbol_count < MAX_SYMBOLS) {
                            /* Check if symbol already exists in the table */
                            symbol_found = 0;
                            for (i = 0; i < symbol_count && !symbol_found;
                                 i++) {
                                if (strcmp(symbol_table[i].symbol_name,
                                           symbol_name) == 0) {
                                    printf("Error in line %d: Symbol '%s' "
                                           "already defined\n",
                                           line_number, symbol_name);
                                    error_count++;
                                    symbol_found = 1;
                                }
                            }

                            if (!symbol_found) {
                                strcpy(symbol_table[symbol_count].symbol_name,
                                       symbol_name);
                                symbol_table[symbol_count].symbol_value = IC;
                                symbol_table[symbol_count].symbol_type =
                                    SYMBOL_TYPE_CODE;
                                symbol_count++;
                            }
                        } else {
                            printf("Error: Symbol table full, max %d symbols\n",
                                   MAX_SYMBOLS);
                            error_count++;
                        }
                    }

                    /* 12. Find the instruction's opcode in the opcode table */
                    instruction = field;
                    if (search_command_in_table(instruction)) {
                        /* 13. Analyze the operands of the instruction */
                        operand1 = split_line_to_fields(NULL, 0, " \t\n,");
                        operand2 = split_line_to_fields(NULL, 0, " \t\n,");

                        /* Calculate the total word count (L) for this
                         * instruction */
                        L = calc_num_of_words(instruction, operand1, operand2);

                        /* 14. Build the binary code for the instruction */
                        operand1_mode =
                            operand1 ? get_addressing_mode(operand1) : -1;
                        operand2_mode =
                            operand2 ? get_addressing_mode(operand2) : -1;

                        /* Generate binary code for the instruction */
                        word_to_binary(search_command_in_table(instruction),
                                       operand1_mode, operand2_mode, code,
                                       &code_index);

                        /* 15. Save current IC and L values along with the
                         * machine code generated */
                        save_values_with_binary(code, code_index, NULL, IC);

                        /* 16. Update IC = IC + L, and go to step 2 */
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

        /* Clean up */
        fclose(input_pre_assembled);

        /* Print summary */
        printf("First pass finished.\n");
        printf("ICF = %d, DCF = %d\n", ICF, DCF);
        printf("Symbol count = %d\n", symbol_count);
        printf("Error count = %d\n", error_count);
    }
    return 0;
}

/* Helper functions */

char *split_line_to_fields(char *line, int field_number,
                           const char *delimiters) {
    static char *last_ptr = NULL;
    char *token;
    int current_field = 0;

    /* If line is not NULL, this is the first call on a new line */
    if (line != NULL) {
        last_ptr = line;
    }

    /* Get the token */
    token = strtok(last_ptr, delimiters);
    last_ptr = NULL;

    /* If field_number is 0, return the first token */
    if (field_number == 0) {
        return token;
    }

    /* Otherwise, walk through the tokens until we reach the requested field */
    while (token != NULL && current_field < field_number) {
        token = strtok(NULL, delimiters);
        current_field++;
    }

    return token;
}

int is_symbol(char *field) {
    /* Check if the field ends with a colon, indicating a label/symbol */
    int len = strlen(field);

    /* Check if the field is a valid symbol name */
    if (len > 0 && field[len - 1] == ':') {
        field[len - 1] = '\0'; /* Remove the colon */
        return !is_reserved_word(field);
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
    /* Base word count is 1 for the instruction itself */
    int word_count = 1;

    /* Add words for operands based on their addressing modes */
    if (operand1) {
        int mode = get_addressing_mode(operand1);
        word_count += get_operand_words(mode);
    }

    if (operand2) {
        int mode = get_addressing_mode(operand2);
        word_count += get_operand_words(mode);
    }

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
    /* Placeholder implementation */
    /* This would generate the binary representation of the instruction */
    /* For now, just store dummy values */

    /* First word: op-code, addressing modes, etc. */
    code[(*code_index)++] = (instruction_index << 8) | ((operand1_mode & 0x3) << 4) | ((operand2_mode & 0x3) << 2);

    /* Additional words would be added for operands */
}

void save_values_with_binary(unsigned int *code, int word_count, int *memory, int current_ic) {
    /* Placeholder implementation */
    /* This would save the generated code to memory */
    /* For now, just print the values */
    int i;
    for (i = 0; i < word_count; i++) {
        printf("Memory[%d] = 0x%04X\n", current_ic + i, code[i]);
    }
}

void add_lcf_to_data(int offset) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (symbol_table[i].symbol_type == SYMBOL_TYPE_DATA) {
            symbol_table[i].symbol_value += offset;
        }
    }
}
