#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H

#include "../shared/types.h"
#include "../shared/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function prototypes */

/* Main function for first pass */
int first_pass(const char *src);

/* Symbol and label handling */
int is_symbol(char *field);
int is_storage(char *field);
int is_extern_or_entry(char *field);
int is_reserved_word(const char *word);

/* Symbol table functions */
int add_code_to_table(char *symbol_name);
void add_lcf_to_data(int offset);

/* Instruction processing */
int search_command_in_table(char *command);
int calc_num_of_words(char *instruction, char *operand1, char *operand2);
int get_addressing_mode(char *operand);
int get_operand_words(int addressing_mode);
void word_to_binary(int instruction_index, int operand1_mode, int operand2_mode,
                    unsigned int *code, int *code_index);
void save_values_with_binary(unsigned int *code, int word_count, int *memory,
                             int current_ic);

/* Utility functions */
char *split_line_to_fields(char *line, int field_number,
                           const char *delimiters);

#endif /* ASSEMBLER_FIRST_PASS_H */