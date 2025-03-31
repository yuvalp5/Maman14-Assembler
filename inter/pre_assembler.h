/**
 * @brief Pre-Assembler implementation as a master method and subfunctions
 */

#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H 1

/**
 * @brief Main function for pre-assembler - processes macros in source file and
 * @param src Path to the source assembly file
 * @param dest Path for the expanded output file
 * @return 0 on success, non-zero on failure
 */
int pre_assembler(const char *file_basename);

/**
 * @brief Extract the first word from a line
 * @param line The input line
 * @param word Buffer to store the extracted first word
 */
void extract_first_word(const char *line, char *word);

/**
 * @brief Check if a macro name is valid (not a reserved word)
 * @param name The macro name to check
 * @return 1 if valid, 0 if invalid
 */
int is_valid_macro_name(const char *name);

#endif