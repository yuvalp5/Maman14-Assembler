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
 * Concatenes two strings to a new one (used to add filename extensions)
 * @param str1 First string
 * @param str2 Second string
 * @return Concatenated string
 */
char *add_ext(const char *str1, const char *str2);

#endif