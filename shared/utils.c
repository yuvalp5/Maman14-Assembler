/**
 * @brief Shared methods for project
 */

#ifndef UTILS_H
#define UTILS_H 1

#include <stdio.h>

/**
 * @brief Prints a message and logs it to a file
 * @param text The message to be printed and logged
 * @param format The format string for the message
 * @return 0 on success; 1 on failure
 */
int print_and_log(char *text, char *format);

/**
 * @brief Exit gracefully with provided code and message, closing log file
 * @param exit_code The exit code to be returned
 * @param stop Whether to exit() the program or not
 */
void exit_graceful(int exit_code, int stop);

/**
 * @brief Check if a macro name is valid (not a reserved word)
 * @param name The macro name to check
 * @return 1 if valid, 0 if invalid
 */
int is_valid_macro_name(const char *name);

/* Function declarations */
int get_addressing_mode(char *operand);

#endif