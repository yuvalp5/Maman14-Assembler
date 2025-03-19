/**
 * @brief Shared methods for project
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FILE_LOC "./io/log.txt"
#define MAX_LINE_LEN 256
#define MAX_MACRO_NAME_LEN 31      // Maximum length of a macro name
#define MAX_MACRO_CONTENT_LEN 1000 // Maximum length of macro content
#define MAX_MACROS 50 // Maximum number of macros that can be stored
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"

/**
 * @brief Reads a line from a file and returns it as a string
 * @param text text to separate into fields
 * @return line separated into fields
 */
char *line_to_fields(char *text);

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
 * @param log_file The file pointer to the log file
 * @param stop Whether to exit() the program or not
 */
void exit_graceful(int exit_code, FILE *log_file, int stop);