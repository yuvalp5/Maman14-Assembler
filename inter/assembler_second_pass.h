/**
 * @brief Second pass implementation as a master method and subfunctions
 */

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H 1

#include "types.h"
/* Addressing modes for operands */
typedef enum {
    IMMEDIATE = 0,
    DIRECT = 1,
    REGISTER_DIRECT = 2,
    RELATIVE = 3
} AddressingMode;

/**
 * @brief Main function for second pass - generates machine code and output
 * files
 * @param src_file Path to the source assembly file
 * @param dest_file Base path for the output files
 * @return 0 on success, non-zero on failure
 */
int second_pass(const char *src_file, const char *dest_file);

/**
 * @brief Process a single line in second pass
 * @param line The line to process
 * @param memory Array to store machine code
 * @return 0 on success, non-zero on failure
 */
int process_line_second_pass(char *line, unsigned int *memory);

/**
 * @brief Complete instruction encoding with symbol resolution
 * @param instruction The instruction to encode
 * @param operands The operands string
 * @param memory Array to store machine code
 * @param current_ic Current instruction counter
 * @return 0 on success, non-zero on failure
 */
int complete_instruction_encoding(char *instruction, char *operands,
                                  unsigned int *memory, int current_ic);

/**
 * @brief Generate output files (.ob, .ext, .ent)
 * @param dest_file Base path for output files
 * @param memory Array containing machine code
 * @return 0 on success, non-zero on failure
 */
int generate_output_files(const char *dest_file, unsigned int *memory);

/**
 * @brief Handle entry declaration
 * @param operand The entry symbol
 * @return 0 on success, non-zero on failure
 */
int handle_entry_declaration(char *operand);

/**
 * @brief Update externals file with symbol reference
 * @param symbol The external symbol
 * @param address The address where the symbol is used
 * @return 0 on success, non-zero on failure
 */
int update_externals_file(const char *symbol, int address);

/**
 * @brief Check if a line is empty or a comment
 * @param line The line to check
 * @return 1 if empty or comment, 0 otherwise
 */
int is_empty_or_comment_second_pass(const char *line);

/**
 * @brief Split a line into fields (label, instruction, operands) for second
 * pass
 * @param line The line to split
 * @param fields Array to store the fields
 * @return 0 on success, non-zero on failure
 */
int split_line_to_fields_second_pass(char *line, char **fields);

/**
 * @brief Get addressing mode for an operand in second pass
 * @param operand The operand to analyze
 * @return Addressing mode enum value
 */
AddressingMode get_addressing_mode_second_pass(const char *operand);

#endif
