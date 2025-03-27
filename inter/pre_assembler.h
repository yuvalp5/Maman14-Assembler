/**
 * @brief Pre-Assembler implementation for the Assembler project. Devided into
 * master function and supporting functions
 */

#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H 1

#include "../shared/definitions.h"
#include "../shared/types.h"

/**
 * @brief Main function for pre-assembler - processes macros in source file and
 * @param src Path to the source assembly file (.as)
 * @param dest Path for the expanded output file (.am)
 * @return 0 on success, non-zero on failure
 */
int pre_assembler(char *src, char *dest);

/**
 * @brief Extracts macro name from a macro definition line
 * @param line The macro definition line (starting with "mcro")
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
 * @brief Add a new macro to the macro table
 * @param name Name of the macro to add
 * @return 1 on success, 0 on failure
 */
int add_macro(char *name);

/**
 * @brief Append a line to an existing macro's content
 * @param name Name of the macro to append to
 * @param line Line to append to the macro's content
 * @return 1 on success, 0 on failure
 */
int append_to_macro(const char *name, const char *line);

/**
 * @brief Get the content of a macro by name
 * @param name Name of the macro to look up
 * @return Pointer to the macro's content, or NULL if macro not found
 */
char *get_macro_content(const char *name);

/**
 * @brief Check if a macro name is valid (not a reserved word)
 * @param name The macro name to check
 * @return 1 if valid, 0 if invalid
 */
int is_valid_macro_name(const char *name);

#endif