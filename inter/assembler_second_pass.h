/**
 * @brief Second pass implementation
 * Handles final code generation and output file creation
 */

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H 1

#include "shared/types.h"

/**
 * @brief Master function for second pass
 * @param object_file Path to the object file to be created
 * @return 1 on success, 0 on failure
 */
int second_pass(const char *object_file);

/**
 * @brief Process a single line for macro definition or usage
 * @param line The line to process
 * @param in_macro Flag indicating if we're inside a macro definition
 * @param current_macro Name of the macro being defined
 * @param macro_content Content of the macro being defined
 * @return 0 on success, non-zero on failure
 */
int process_line_pre_assembler(char *line, int *in_macro, char *current_macro, char *macro_content);

/**
 * @brief Extract macro name from a macro definition line
 * @param line The macro definition line
 * @param macro_name Buffer to store the extracted name
 * @return 0 on success, non-zero on failure
 */
int extract_macro_name(const char *line, char *macro_name);

/**
 * @brief Check if a line is empty or a comment
 * @param line The line to check
 * @return 1 if empty or comment, 0 otherwise
 */
int is_empty_or_comment(const char *line);

#endif
 