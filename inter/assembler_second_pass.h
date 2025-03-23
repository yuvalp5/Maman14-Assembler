/* Second pass header for the assembler
 * Handles the generation of machine code and output files
 */

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H

#include "../shared/definitions.h"
#include "../shared/types.h"
#include "../shared/utils.h"
#include "assembler_first_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Symbol table entry structure (should be in types.h ideally) */
typedef struct {
    char name[MAX_LINE_LEN];
    int address;
    int is_entry;
    int is_external;
} SymbolEntry;

/* Symbol table variables - made extern for testing */
extern SymbolEntry symbol_table[MAX_LINE_LEN];
extern int symbol_count;

/* Main functions */
void second_pass(const char *filename);
int process_line(char *line, int line_number, int *error_count);

/* Line processing */
int parse_line(char *line, char **label, char **command, char **operands);
int is_comment_or_empty(const char *line);
int process_instruction(char *command, char *operands, int line_number,
                        int *error_count);
int process_directive(char *directive, char *operands, int line_number,
                      int *error_count);
int add_entry_symbol(char *symbol, int line_number, int *error_count);

/* Machine code generation */
int generate_machine_code(int opcode, int src_mode, int dest_mode, char *src,
                          char *dest);
int encode_operands(char *src, char *dest, int src_mode, int dest_mode);
int get_opcode(const char *command);
int get_addressing_mode(const char *operand);
int is_valid_addressing_mode(int opcode, int src_mode, int dest_mode);
int is_arithmetic_op(int opcode);
int get_function_code(int opcode);

/* Output generation */
void generate_output_files(const char *filename);
void generate_object_file(const char *filename);
void generate_entry_file(const char *filename);
void generate_external_file(const char *filename);
int has_entry_symbols();
int has_external_symbols();
void add_to_code_segment(int machine_code);

#endif /* ASSEMBLER_SECOND_PASS_H */
