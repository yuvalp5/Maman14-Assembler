/* Second pass header for the assembler
 * Handles the generation of machine code and output files
 */

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler_first_pass.h"
#include "../shared/definitions.h"
#include "../shared/utils.h"

/* Global variables */
/*extern Symbol symbol_table[MAX_LINE_LEN];*/
extern int symbol_count;

/* Function declarations */
int second_pass(const char *filename);
int parse_line(char *line, char **label, char **command, char **operands);
int process_line(char *line, int line_number, int *error_count);
int process_directive(char *directive, char *operands, int line_number,
                      int *error_count);
int add_entry_symbol(char *symbol, int line_number, int *error_count);
int process_instruction(char *command, char *operands, int line_number,
                        int *error_count);
void add_to_code_segment(int machine_code);
int get_opcode(const char *command);
int is_valid_addressing_mode(int opcode, int src_mode, int dest_mode);
int is_arithmetic_op(int opcode);
int get_function_code(int opcode);
int encode_operands(char *src, char *dest, int src_mode, int dest_mode);
int generate_machine_code(int opcode, int src_mode, int dest_mode, char *src,
                          char *dest);
int has_entry_symbols(void);
int has_external_symbols(void);
void generate_output_files(const char *filename);
void generate_object_file(const char *filename);
void generate_entry_file(const char *filename);
void generate_external_file(const char *filename);

#endif /* ASSEMBLER_SECOND_PASS_H */
