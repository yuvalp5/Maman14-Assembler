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
int pre_assembler(char *src, char *dest);

/**
 * @brief Extracts macro name from a macro definition line
 * @param line The macro definition line
 * @param macro_name Buffer to store the extracted name
 * @return 1 on success, 0 on failure
 */
int extract_macro_name(const char *line, char *macro_name);

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