/**
 * @brief Shared methods for project
 */

#ifndef UTILS_H
#define UTILS_H 1

#include <stdio.h>

/**
 * @brief Prints a message and logs it to a file
 * @param text The message to be printed and logged
 * @param log_file The file pointer to the log file
 * @return 0 on success; 1 on failure
 */
int print_and_log(char *text);

/**
 * @brief Exit gracefully with provided code and message, closing log file
 * @param exit_code The exit code to be returned
 * @param stop Whether to exit() the program or not
 */
void exit_graceful(int exit_code, int stop);

#endif