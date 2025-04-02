/**
 * @brief Shared methods for project
 */

#ifndef UTILS_H
#define UTILS_H 1

#include <stdio.h>

/**
 * @brief Exit gracefully with provided code and message, closing log file
 * @param exit_code The exit code to be returned
 * @param stop Whether to exit() the program or not
 */
void exit_graceful(int exit_code, int stop);

/**
 * @brief Checks if a string is a reserved keyword
 * @param keyword String to check
 * @return 1 if the string is a reserved keyword; 0 otherwise
 */
int is_reserved_kw(char *keyword);

/**
 * Concatenes two strings to a new one (used to add filename extensions)
 * @param str1 First string
 * @param str2 Second string
 * @return Concatenated string
 */
char *add_ext(const char *str1, const char *str2);

/**
 * @brief Extracts macro name from a macro definition line
 * @param line The macro definition line
 * @param macro_name Buffer to store the extracted name
 * @return 1 on success, 0 on failure
 */
int extract_macro_name(const char *line, char *macro_name);

/**
 * @brief Checks if a name is a reserved word
 * @param name The name to check
 * @return 1 if the name is a reserved word, 0 otherwise
 */
int is_reserved_word(const char *name);

#endif