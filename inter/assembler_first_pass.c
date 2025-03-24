#include "assembler_first_pass.h"

int DC = 0;
int IC = 100;
char line[MAX_LINE_LEN];
char line_copy[MAX_LINE_LEN]; 
FILE *input_pre_assembled = NULL;
Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;
int error_count = 0;
int line_number = 0;
int ICF, DCF;



void first_pass(const char *src) {
    /**
     * 1. Initialize:
   - IC = 100 (instruction counter starts at 100 decimal) - done
   - DC = 0 (data counter starts at 0) - done
   - open source file - done
   */
    output_pre_assembled = fopen(dest, "w");
    if (!output_pre_assembled) {
        printf("Error: Unable to open intput file\n");
        exit_graceful(1, 1);

  /**
   * 2. Read the next line from the input file:
   - If end of file, go to step 17
   */
  while (fgets(line, MAX_LINE_LEN, input_pre_assembled) != NULL) {
    line_number++;
    /* Process the line (implementation of steps 3-16 will go here) */
    if (feof(input_pre_assembled)) {
        break; /* End of file, we'll not use break at the end */

    line_copy = split_line_to_fields(line, 0, " :");
    if (is_symbol(line_copy)) {
        is_symbol = 1;
    }
    if (is_storage(line_copy)) {
        /* add the symbol to the symbol table:
   - Symbol name = the label
   - Symbol value = current DC
   - Symbol type = data
   - Encode the data values in memory, update the data counter (DC) according to the number of values, and go to step 2*/
    }

    if (is_extern_or_entry(line_copy)) {
     /*if its .entry, skip it (will be handled in second pass) and go to step 2*/
     /*If directive is .extern, add the symbol to the symbol table:
    - Symbol name = the operand of .extern
    - Symbol value = 0
    - Symbol type = external
    - Go to step 2 */
    }

    if (is_instruction(line_copy)) {
        /* This is an instruction line. If symbol definition found, add the symbol to the symbol table:
    - Symbol name = the label
    - Symbol value = current IC
    - Symbol type = code
    - Raise error if the symbol is already in the table
    */
    }

    if (search_command_in_table(line_copy) == 1) {
        /* Find the instruction's opcode in the opcode table:*/
    }
    else{

    }




/**

12. Find the instruction's opcode in the opcode table:
    - If not found, report an error in the instruction name
13. Analyze the operands of the instruction:
    - Determine addressing modes of operands
    - Calculate the total word count (L) for this instruction
14. Build the binary code for:
    - The first word of the instruction
    - Any immediate operand value words
15. Save current IC and L values along with the machine code generated
16. Update IC = IC + L, and go to step 2

18. Update data symbols in the symbol table by adding ICF to their values
    (This adjusts data symbol addresses to account for the code section)
19. Begin second pass
     */

    }
    /* 17. End of first pass, save final values of IC and DC as ICF and DCF */
    ICF = IC;
    DCF = DC;
}

/* split line to fields, return the field number, move later to utils */
char* split_line_to_fields(char *line, int field_number, const char *delimiters) {
    char *token;
    int current_field = 0;
    
    /* Get the first token */
    token = strtok(line, delimiters);
    
    /* Walk through the tokens until we reach the requested field */
    while (token != NULL) {
        if (current_field == field_number) {
            return token;
        }
        token = strtok(NULL, delimiters);
        current_field++;
    }
    
    /* Field not found */
    return NULL;
}

/* 3- almost same as macro check from pre_assembler.c */
/* todo- maybe same check as from pre_assembler but more return options */
int is_symbol(int field) {
    /* New definition */
    if (!strcmp(field, "mcro")) {
        return 1;
    }
    /* Call to existing */
    int i;
    for (i = 0; i < macro_table.len; i++) { /* I honestly hate this */
        if (!strcmp(macro_table.items[i].name, field)) {
            return 1;
        }
    }
    /* Not symbol */
    return 0;
}

/* 5*/
int is_storage(int field) {
    /* todo yuval- movbe this check into master method. */
    /* the check is just the if statement */
    if (!strcmp(field, ".data") || !strcmp(field, ".string")) {
        return 1;
    }
    return 0;
}

/* 6+7 */
int add_symbol_to_table(
    int name, int val); /* todo oren- implement later when types are done */

/* 8+9+10 */
void is_extern_or_entry(
    int field) { /* todo yuval- movbe this check into master method. */
    /* the check is just the if statement */
    if (!strcmp(field, ".extern") || !strcmp(field, ".entry")) {
        return 1;
    }
    return 0;
};

/* 11 - if everything else is false */
void add_code_to_table(int field);

/* 12 */
void search_command_in_table(int field);

/* 13 */
void calc_num_of_words(int field);

/* 14 */
void word_to_binary(int field);

/* 15 */
void save_values_with_binary(
    int field); /* TODO: need to understand WTF this is */

/* 19 */
void add_lcf_to_data(int field);
