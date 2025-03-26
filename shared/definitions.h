/**
 * @brief Wrapper component to be accessed by user. Main entry point used to
 * recive input and pass it on through all stages as well as to print the output
 * to the user. Included are also definitions for use in the project.
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H 1

/* File extensions */
/*const char *SOURCE_FILE_EXT = ".as";*/
/*const char *PRE_ASSEMBLED_FILE_EXT = ".am";*/
/*const char *OBJECT_FILE_EXT = ".ob";*/
/*const char *OUTPUT_FILE_EXT = ".out";*/
/*const char *ENTRY_FILE_EXT = ".ent";*/
/*const char *EXTERN_FILE_EXT = ".ext";*/
/*const char *PRE_ASSEMBLED_SAVE_LOC = "./io/pre_assembler.txt";*/

/* Macro keywords */
const char *MACRO_START_KW = "mcro";
const char *MACRO_END_KW = "mcroend";

/* Project-wide definitions */
#define LOG_FILE_LOC "./io/log.txt"
#define MAX_MACRO_NAME_LEN 31
#define MAX_MACRO_CONTENT_LEN 1000
#define MAX_MACROS 50

/* Constants */
#define MAX_LINE_LEN 80
#define MAX_SYMBOL_LENGTH 31
#define MAX_SYMBOLS 256

/* File extensions */
extern const char *SOURCE_FILE_EXT;
extern const char *PRE_ASSEMBLED_FILE_EXT;
extern const char *OBJECT_FILE_EXT;
extern const char *OUTPUT_FILE_EXT;
extern const char *ENTRY_FILE_EXT;
extern const char *EXTERN_FILE_EXT;
extern const char *PRE_ASSEMBLED_SAVE_LOC;

/* Macro keywords */
extern const char *MACRO_START_KW;
extern const char *MACRO_END_KW;

#endif