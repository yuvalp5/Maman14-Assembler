/**
 * @brief Second pass of the assembler
 * Handles the generation of machine code and output files
 */

#include "assembler_second_pass.h"

/* Global variables for the second pass */
static int IC = 100;              /* Instruction Counter starts at 100 */
static int DC = 0;                /* Data Counter */
static int *code_segment = NULL;  /* Array to hold machine code */
static int code_segment_size = 0; /* Current size of code segment */
static int error_found = 0;       /* Flag for indicating if errors were found */

/* Symbol table (should be shared between passes) */
SymbolEntry symbol_table[MAX_LINE_LEN];
int symbol_count = 0;
static char entry_symbols[MAX_LINE_LEN][MAX_LINE_LEN];
static int entry_count = 0;
static char extern_symbols[MAX_LINE_LEN][MAX_LINE_LEN];
static int extern_addresses[MAX_LINE_LEN];
static int extern_count = 0;

/**
 * @brief Main function for the second pass
 * @param filename Name of the .am file to process
 */
void second_pass(const char *filename) {
    FILE *source_file;
    char line[MAX_LINE_LEN];
    int line_number = 0;
    int error_count = 0;
    char am_filename[MAX_LINE_LEN];

    /* Create .am filename */
    strcpy(am_filename, filename);
    strcat(am_filename, PRE_ASSEMBLED_FILE_EXT);

    printf("Starting second pass for file: %s\n", am_filename);

    /* Initialize code segment */
    code_segment = (int *)malloc(MAX_LINE_LEN * sizeof(int));
    if (!code_segment) {
        printf("Error: Memory allocation failed for code segment\n");
        return;
    }

    /* Open the .am file */
    source_file = fopen(am_filename, "r");
    if (!source_file) {
        printf("Error: Unable to open file %s\n", am_filename);
        free(code_segment);
        return;
    }

    /* Process each line */
    while (fgets(line, MAX_LINE_LEN, source_file)) {
        line_number++;

        /* Skip comments and empty lines */
        if (is_comment_or_empty(line)) {
            continue;
        }

        /* Process the line */
        process_line(line, line_number, &error_count);
    }

    /* Generate output files if no errors */
    if (error_count == 0) {
        generate_output_files(filename);
    } else {
        printf(
            "Found %d errors during second pass. No output files generated.\n",
            error_count);
    }

    /* Cleanup */
    fclose(source_file);
    free(code_segment);
}

/**
 * @brief Check if a line is a comment or empty
 * @param line Line to check
 * @return 1 if comment or empty, 0 otherwise
 */
int is_comment_or_empty(const char *line) {
    int i = 0;

    /* Skip leading whitespace */
    while (line[i] && (line[i] == ' ' || line[i] == '\t')) {
        i++;
    }

    /* Check if comment or empty */
    return (line[i] == ';' || line[i] == '\n' || line[i] == '\0');
}

/**
 * @brief Parse a line into label, command, and operands
 * @param line Line to parse
 * @param label Pointer to store label
 * @param command Pointer to store command
 * @param operands Pointer to store operands
 * @return 1 if successful, 0 otherwise
 */
int parse_line(char *line, char **label, char **command, char **operands) {
    char *token;

    /* Initialize pointers */
    *label = NULL;
    *command = NULL;
    *operands = NULL;

    /* Remove newline */
    line[strcspn(line, "\n")] = '\0';

    /* Check for label */
    if ((token = strchr(line, ':')) != NULL) {
        *token = '\0';
        *label = line;
        line = token + 1;

        /* Skip whitespace */
        while (*line && (*line == ' ' || *line == '\t')) {
            line++;
        }
    }

    /* Get command */
    token = line;
    while (*line && *line != ' ' && *line != '\t') {
        line++;
    }

    if (*line) {
        *line = '\0';
        *command = token;
        line++;

        /* Skip whitespace */
        while (*line && (*line == ' ' || *line == '\t')) {
            line++;
        }

        /* Get operands if any */
        if (*line) {
            *operands = line;
        }
    } else {
        *command = token;
    }

    return 1;
}

/**
 * @brief Process a line during the second pass
 * @param line Line to process
 * @param line_number Line number for error reporting
 * @param error_count Pointer to error counter
 * @return 1 if successful, 0 otherwise
 */
int process_line(char *line, int line_number, int *error_count) {
    char *label;
    char *command;
    char *operands;

    /* Parse the line */
    if (!parse_line(line, &label, &command, &operands)) {
        printf("Error: Unable to parse line %d\n", line_number);
        (*error_count)++;
        return 0;
    }

    /* Skip if only label */
    if (label && !command) {
        return 1;
    }

    /* Check if directive */
    if (command[0] == '.') {
        return process_directive(command, operands, line_number, error_count);
    }

    /* Process instruction */
    return process_instruction(command, operands, line_number, error_count);
}

/**
 * @brief Process a directive during the second pass
 * @param directive Directive to process
 * @param operands Operands for the directive
 * @param line_number Line number for error reporting
 * @param error_count Pointer to error counter
 * @return 1 if successful, 0 otherwise
 */
int process_directive(char *directive, char *operands, int line_number,
                      int *error_count) {
    /* Only process .entry directives in second pass */
    if (strcmp(directive, ".entry") == 0) {
        return add_entry_symbol(operands, line_number, error_count);
    }

    /* Ignore other directives in second pass */
    return 1;
}

/**
 * @brief Add an entry symbol to the symbol table
 * @param symbol Symbol name
 * @param line_number Line number for error reporting
 * @param error_count Pointer to error counter
 * @return 1 if successful, 0 otherwise
 */
int add_entry_symbol(char *symbol, int line_number, int *error_count) {
    int i;

    /* Trim whitespace */
    while (*symbol && (*symbol == ' ' || *symbol == '\t')) {
        symbol++;
    }

    /* Find symbol in table */
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, symbol) == 0) {
            /* Cannot be both external and entry */
            if (symbol_table[i].is_external) {
                printf(
                    "Error line %d: Symbol %s is already defined as external\n",
                    line_number, symbol);
                (*error_count)++;
                return 0;
            }

            /* Mark as entry */
            symbol_table[i].is_entry = 1;

            /* Add to entry symbols list */
            strcpy(entry_symbols[entry_count], symbol);
            entry_count++;

            return 1;
        }
    }

    /* Symbol not found */
    printf("Error line %d: Symbol %s not found in symbol table\n", line_number,
           symbol);
    (*error_count)++;
    return 0;
}

/**
 * @brief Process an instruction during the second pass
 * @param command Instruction command
 * @param operands Instruction operands
 * @param line_number Line number for error reporting
 * @param error_count Pointer to error counter
 * @return 1 if successful, 0 otherwise
 */
int process_instruction(char *command, char *operands, int line_number,
                        int *error_count) {
    int opcode = get_opcode(command);
    char src[MAX_LINE_LEN] = {0};
    char dest[MAX_LINE_LEN] = {0};
    int src_mode = 0;
    int dest_mode = 0;
    int machine_code = 0;
    char *src_ptr, *dest_ptr;

    /* Check if valid opcode */
    if (opcode < 0) {
        printf("Error line %d: Invalid instruction %s\n", line_number, command);
        (*error_count)++;
        return 0;
    }

    /* Parse operands */
    if (operands) {
        char *comma = strchr(operands, ',');

        /* Two operands */
        if (comma) {
            *comma = '\0';
            strcpy(src, operands);
            strcpy(dest, comma + 1);

            /* Trim whitespace from dest using a pointer */
            dest_ptr = dest;
            while (*dest_ptr && (*dest_ptr == ' ' || *dest_ptr == '\t')) {
                dest_ptr++;
            }
            if (dest_ptr != dest) {
                memmove(dest, dest_ptr, strlen(dest_ptr) + 1);
            }
        }
        /* One operand */
        else {
            strcpy(dest, operands);
        }

        /* Trim whitespace from src using a pointer */
        src_ptr = src;
        while (*src_ptr && (*src_ptr == ' ' || *src_ptr == '\t')) {
            src_ptr++;
        }
        if (src_ptr != src) {
            memmove(src, src_ptr, strlen(src_ptr) + 1);
        }
    }

    /* Get addressing modes */
    if (src[0]) {
        src_mode = get_addressing_mode(src);
    }

    dest_mode = get_addressing_mode(dest);

    /* Validate addressing modes */
    if (!is_valid_addressing_mode(opcode, src_mode, dest_mode)) {
        printf("Error line %d: Invalid addressing mode for instruction %s\n",
               line_number, command);
        (*error_count)++;
        return 0;
    }

    /* Generate machine code */
    machine_code =
        generate_machine_code(opcode, src_mode, dest_mode, src, dest);

    /* Add to code segment */
    add_to_code_segment(machine_code);

    return 1;
}

/**
 * @brief Add machine code to the code segment
 * @param machine_code Machine code to add
 */
void add_to_code_segment(int machine_code) {
    code_segment[code_segment_size] = machine_code;
    code_segment_size++;
}

/**
 * @brief Get the opcode for a command
 * @param command Command to get opcode for
 * @return Opcode if found, -1 otherwise
 */
int get_opcode(const char *command) {
    /* Standard assembly instructions */
    if (strcmp(command, "mov") == 0)
        return 0;
    if (strcmp(command, "cmp") == 0)
        return 1;
    if (strcmp(command, "add") == 0)
        return 2;
    if (strcmp(command, "sub") == 0)
        return 3;
    if (strcmp(command, "lea") == 0)
        return 4;
    if (strcmp(command, "clr") == 0)
        return 5;
    if (strcmp(command, "not") == 0)
        return 6;
    if (strcmp(command, "inc") == 0)
        return 7;
    if (strcmp(command, "dec") == 0)
        return 8;
    if (strcmp(command, "jmp") == 0)
        return 9;
    if (strcmp(command, "bne") == 0)
        return 10;
    if (strcmp(command, "jsr") == 0)
        return 11;
    if (strcmp(command, "red") == 0)
        return 12;
    if (strcmp(command, "prn") == 0)
        return 13;
    if (strcmp(command, "rts") == 0)
        return 14;
    if (strcmp(command, "stop") == 0)
        return 15;

    return -1;
}

/**
 * @brief Get the addressing mode for an operand
 * @param operand Operand to get addressing mode for
 * @return Addressing mode (0-3)
 */
int get_addressing_mode(const char *operand) {
    /* Immediate addressing mode (e.g., #1) */
    if (operand[0] == '#') {
        return 0;
    }

    /* Register addressing mode (e.g., r0-r7) */
    if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7' &&
        operand[2] == '\0') {
        return 3;
    }

    /* Relative addressing mode (e.g., &LABEL) */
    if (operand[0] == '&') {
        return 2;
    }

    /* Direct addressing mode (e.g., LABEL) */
    return 1;
}

/**
 * @brief Check if the addressing mode is valid for an opcode
 * @param opcode Instruction opcode
 * @param src_mode Source addressing mode
 * @param dest_mode Destination addressing mode
 * @return 1 if valid, 0 otherwise
 */
int is_valid_addressing_mode(int opcode, int src_mode, int dest_mode) {
    /* Instructions with no operands: rts, stop */
    if (opcode == 14 || opcode == 15) { /* rts, stop */
        /* For no-operand instructions, we expect empty operands */
        return 1;
    }

    /* Instructions with only destination operand */
    if (opcode >= 5 &&
        opcode <= 13) { /* clr, not, inc, dec, jmp, bne, jsr, red, prn */
        return src_mode == 0 && dest_mode > 0;
    }

    /* lea instruction only allows direct addressing for source */
    if (opcode == 4) { /* lea */
        /* Allow direct addressing mode for source and any mode except immediate
         * for dest */
        return src_mode == 1 && dest_mode != 0;
    }

    /* Other instructions */
    return src_mode >= 0 && dest_mode > 0;
}

/**
 * @brief Check if an opcode is for an arithmetic operation
 * @param opcode Instruction opcode
 * @return 1 if arithmetic, 0 otherwise
 */
int is_arithmetic_op(int opcode) {
    return opcode == 2 || opcode == 3; /* add, sub */
}

/**
 * @brief Get the function code for an arithmetic operation
 * @param opcode Instruction opcode
 * @return Function code
 */
int get_function_code(int opcode) {
    if (opcode == 2)
        return 1; /* add */
    if (opcode == 3)
        return 2; /* sub */
    return 0;
}

/**
 * @brief Encode the operands for an instruction
 * @param src Source operand
 * @param dest Destination operand
 * @param src_mode Source addressing mode
 * @param dest_mode Destination addressing mode
 * @return Encoded operands
 */
int encode_operands(char *src, char *dest, int src_mode, int dest_mode) {
    int encoded = 0;
    int i;

    /* Encode source operand */
    switch (src_mode) {
    case 0:                                /* Immediate */
        encoded |= atoi(src + 1) & 0x3FFF; /* 14 bits */
        break;
    case 1: /* Direct */
        /* Find symbol in table */
        for (i = 0; i < symbol_count; i++) {
            if (strcmp(symbol_table[i].name, src) == 0) {
                encoded |= symbol_table[i].address & 0x3FFF;

                /* Handle external symbol */
                if (symbol_table[i].is_external) {
                    strcpy(extern_symbols[extern_count], src);
                    extern_addresses[extern_count] = IC + code_segment_size;
                    extern_count++;
                }

                break;
            }
        }
        break;
    case 2: /* Relative */
        /* Find symbol in table */
        for (i = 0; i < symbol_count; i++) {
            if (strcmp(symbol_table[i].name, src + 1) == 0) {
                encoded |=
                    (symbol_table[i].address - (IC + code_segment_size)) &
                    0x3FFF;
                break;
            }
        }
        break;
    case 3: /* Register */
        encoded |=
            ((src[1] - '0') << 10) & 0x3FFF; /* Register number in bits 10-12 */
        break;
    }

    /* Encode destination operand */
    switch (dest_mode) {
    case 1: /* Direct */
        /* Find symbol in table */
        for (i = 0; i < symbol_count; i++) {
            if (strcmp(symbol_table[i].name, dest) == 0) {
                encoded |= symbol_table[i].address & 0x3FF; /* Lower 10 bits */

                /* Handle external symbol */
                if (symbol_table[i].is_external) {
                    strcpy(extern_symbols[extern_count], dest);
                    extern_addresses[extern_count] = IC + code_segment_size;
                    extern_count++;
                }

                break;
            }
        }
        break;
    case 2: /* Relative */
        /* Find symbol in table */
        for (i = 0; i < symbol_count; i++) {
            if (strcmp(symbol_table[i].name, dest + 1) == 0) {
                encoded |=
                    (symbol_table[i].address - (IC + code_segment_size)) &
                    0x3FF;
                break;
            }
        }
        break;
    case 3:                               /* Register */
        encoded |= (dest[1] - '0') & 0x7; /* Register number in bits 0-2 */
        break;
    }

    return encoded;
}

/**
 * @brief Generate machine code for an instruction
 * @param opcode Instruction opcode
 * @param src_mode Source addressing mode
 * @param dest_mode Destination addressing mode
 * @param src Source operand
 * @param dest Destination operand
 * @return Machine code for the instruction
 */
int generate_machine_code(int opcode, int src_mode, int dest_mode, char *src,
                          char *dest) {
    int machine_code = 0;

    /* Set opcode (4 bits) */
    machine_code |= (opcode & 0xF) << 20;

    /* Set source addressing mode (2 bits) */
    machine_code |= (src_mode & 0x3) << 18;

    /* Set destination addressing mode (2 bits) */
    machine_code |= (dest_mode & 0x3) << 16;

    /* Set function code for arithmetic/logical operations (2 bits) */
    if (is_arithmetic_op(opcode)) {
        machine_code |= (get_function_code(opcode) & 0x3) << 14;
    }

    /* Set operands/addresses (14 bits) */
    machine_code |= encode_operands(src, dest, src_mode, dest_mode);

    return machine_code;
}

/**
 * @brief Check if there are any entry symbols
 * @return 1 if there are entry symbols, 0 otherwise
 */
int has_entry_symbols() { return entry_count > 0; }

/**
 * @brief Check if there are any external symbols
 * @return 1 if there are external symbols, 0 otherwise
 */
int has_external_symbols() { return extern_count > 0; }

/**
 * @brief Generate all output files
 * @param filename Base filename for output files
 */
void generate_output_files(const char *filename) {
    /* Generate object file */
    generate_object_file(filename);

    /* Generate entry file if there are entry symbols */
    if (has_entry_symbols()) {
        generate_entry_file(filename);
    }

    /* Generate external file if there are external symbols */
    if (has_external_symbols()) {
        generate_external_file(filename);
    }
}

/**
 * @brief Generate the object file
 * @param filename Base filename for output files
 */
void generate_object_file(const char *filename) {
    FILE *ob_file;
    char ob_filename[MAX_LINE_LEN];
    int i;

    /* Create .ob filename */
    strcpy(ob_filename, filename);
    strcat(ob_filename, OBJECT_FILE_EXT);

    /* Open the file */
    ob_file = fopen(ob_filename, "w");
    if (!ob_file) {
        printf("Error: Unable to create object file %s\n", ob_filename);
        return;
    }

    /* Write header (IC and DC) */
    fprintf(ob_file, "%d %d\n", code_segment_size, DC);

    /* Write code segment */
    for (i = 0; i < code_segment_size; i++) {
        fprintf(ob_file, "%06X\n",
                code_segment[i] & 0xFFFFFF); /* 24-bit hexadecimal */
    }

    /* Close file */
    fclose(ob_file);

    printf("Created object file: %s\n", ob_filename);
}

/**
 * @brief Generate the entry file
 * @param filename Base filename for output files
 */
void generate_entry_file(const char *filename) {
    FILE *ent_file;
    char ent_filename[MAX_LINE_LEN];
    int i, j;

    /* Create .ent filename */
    strcpy(ent_filename, filename);
    strcat(ent_filename, ENTRY_FILE_EXT);

    /* Open the file */
    ent_file = fopen(ent_filename, "w");
    if (!ent_file) {
        printf("Error: Unable to create entry file %s\n", ent_filename);
        return;
    }

    /* Write entry symbols and their addresses */
    for (i = 0; i < entry_count; i++) {
        /* Find symbol in table */
        for (j = 0; j < symbol_count; j++) {
            if (strcmp(symbol_table[j].name, entry_symbols[i]) == 0) {
                fprintf(ent_file, "%s %d\n", entry_symbols[i],
                        symbol_table[j].address);
                break;
            }
        }
    }

    /* Close file */
    fclose(ent_file);

    printf("Created entry file: %s\n", ent_filename);
}

/**
 * @brief Generate the external file
 * @param filename Base filename for output files
 */
void generate_external_file(const char *filename) {
    FILE *ext_file;
    char ext_filename[MAX_LINE_LEN];
    int i;

    /* Create .ext filename */
    strcpy(ext_filename, filename);
    strcat(ext_filename, EXTERN_FILE_EXT);

    /* Open the file */
    ext_file = fopen(ext_filename, "w");
    if (!ext_file) {
        printf("Error: Unable to create external file %s\n", ext_filename);
        return;
    }

    /* Write external symbols and their addresses */
    for (i = 0; i < extern_count; i++) {
        fprintf(ext_file, "%s %d\n", extern_symbols[i], extern_addresses[i]);
    }

    /* Close file */
    fclose(ext_file);

    printf("Created external file: %s\n", ext_filename);
}
