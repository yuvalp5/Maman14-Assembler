/**
 * @brief Wrapper component to be accessed by user. Main entry point used to
 * recive input and pass it on through all stages as well as to print the output
 * to the user. Included are also definitions for use in the project.
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H 1

/* Project-wide definitions */
#define LOG_FILE_LOC "./io/log.txt"
#define MAX_LINE_LEN 256
#define MAX_SYMBOLS 100
#define MAX_SYMBOL_LEN 31
#define MAX_MACRO_NAME_LEN 31
#define MAX_MACRO_CONTENT_LEN 1000
#define MAX_MACROS 50
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"

/* Standard file extenstions for io files */
#define SOURCE_FILE_EXT ".as"
#define PRE_ASSEMBLED_FILE_EXT ".am"
#define OBJECT_FILE_EXT ".ob"
#define OUTPUT_FILE_EXT ".out"
#define ENTRY_FILE_EXT ".ent"
#define EXTERN_FILE_EXT ".ext"
#define PRE_ASSEMBLED_SAVE_LOC "./io/pre_assembler.txt"

#endif