/**
 * @brief Pre-assembler implementation
 * Handles macro expansion in source files
 */

 #ifndef PRE_ASSEMBLER_H
 #define PRE_ASSEMBLER_H 1
 
 #include "types.h"
 
 /**
  * @brief Main function for pre-assembler
  * Processes macros in source file and writes expanded output
  * @param src Path to source assembly file
  * @param dest Path for expanded output file
  * @return 0 on success, non-zero on failure
  */
 int pre_assembler(const char *src, const char *dest);
 
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
 