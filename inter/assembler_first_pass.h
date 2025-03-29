/**
 * @brief First pass implementation
 */

#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H 1

#include "definitions.h"
#include "types.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Symbol types */
#define SYMBOL_TYPE_CODE 1
#define SYMBOL_TYPE_DATA 2
#define SYMBOL_TYPE_EXTERN 3
#define SYMBOL_TYPE_ENTRY 4

/* Addressing modes */
#define ADDR_MODE_IMMEDIATE 0
#define ADDR_MODE_DIRECT 1
#define ADDR_MODE_INDEX 2
#define ADDR_MODE_REGISTER 3

/**
 * @brief Master function for first pass
 * @param src Path to the source assembly file
 * @param dest Path for the expanded output file
 * @return 0 on success, non-zero on failure
 */
int first_pass(const char *src, const char *dest);

/* Symbol and label handling */
int is_symbol(char *field);
int is_storage(char *field);
int is_extern_or_entry(char *field);
int is_reserved_word(const char *word);

/* Symbol table functions */
int add_code_to_table(char *symbol_name);
void add_lcf_to_data(int offset);
void print_symbol_table(void);

/* Instruction processing */
int search_command_in_table(char *command);
int calc_num_of_words(char *instruction, char *operand1, char *operand2);
int get_addressing_mode(char *operand);
int get_operand_words(int addressing_mode);
void word_to_binary(int instruction_index, int operand1_mode, int operand2_mode,
                    unsigned int *code, int *code_index);
void save_values_with_binary(unsigned int *code, int word_count, int *memory,
                             int current_ic);

/* Data storage handling */
int encode_data_storage(const char *directive, char *operands,
                        int *data_memory);

/* Operand validation */
int validate_operand(char *operand, int instruction_index, int operand_num,
                     int addressing_mode);

/* Utility functions */
char *split_line_to_fields(char *line, int field_number,
                           const char *delimiters);

#endif
