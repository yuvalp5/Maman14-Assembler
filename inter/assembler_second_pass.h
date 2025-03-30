/**
 * @brief Second pass of assembler. Handles generation of machine code and
 * output files
 */

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H 1

/**
 * @brief Master function for second pass. Produces machine code and `externals`
 * / `entries` files (if neccesary) and saves final machine code to destination
 * @param src_file Path of file to perform pass on. expected to have completed
 * first_pass
 * @param dest_file Path of file save
 * @return 0 if successful; 1 otherwise
 */
int second_pass(const char *src_file, const char *dest_file);

/**
 * @brief Process a line during second pass
 * @param line The line to process
 * @param memory Array to store machine code
 * @return 0 if successful; 1 otherwise
 */
int process_line_second_pass(char *line, unsigned int *memory);

/**
 * @brief Complete instruction encoding using symbol values
 * @param instruction The instruction to encode
 * @param operands The operands string
 * @param memory Array to store machine code
 * @param current_ic Current instruction counter value
 * @return 0 if successful; 1 otherwise
 */
int complete_instruction_encoding(char *instruction, char *operands,
                                  unsigned int *memory, int current_ic);

/**
 * @brief Generate output files (.ob, .ext, .ent)
 * @param dest_file Base name for output files
 * @param memory Array containing machine code
 * @return 0 if successful; 1 otherwise
 */
int generate_output_files(const char *dest_file, unsigned int *memory);

/**
 * @brief Handle entry declarations during second pass
 * @param operand The entry symbol
 * @return 0 if successful; 1 otherwise
 */
int handle_entry_declaration(char *operand);

/**
 * @brief Update external references file
 * @param symbol Symbol name
 * @param address Memory address where symbol is referenced
 * @return 0 if successful; 1 otherwise
 */
int update_externals_file(const char *symbol, int address);

#endif
