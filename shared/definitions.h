/**
 * @brief Wrapper component to be accessed by user. Main entry point used to
 * recive input and pass it on through all stages as well as to print the output
 * to the user. Included are also definitions for use in the project.
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H 1

/* File handling */
#define SRC_F_EXT ".as"
#define PAS_F_EXT ".am"
#define OBJ_F_EXT ".ob"
#define EXT_F_EXT ".external"
#define ENT_F_EXT ".entry"
#define LOG_FILE_LOC "./io/log.txt"

/* Project-wide definitions */
#define MAX_MACRO_NAME_LEN 31
#define MAX_MACRO_CONTENT_LEN 1000
#define MAX_MACROS 50

/* Constants */
#define MAX_LINE_LEN 80
#define MAX_SYMBOL_LEN 31
#define MAX_SYMBOLS 256
#define MAX_MEMORY_SIZE 4096

/* Symbol types */
#define SYMBOL_TYPE_CODE 1
#define SYMBOL_TYPE_DATA 2
#define SYMBOL_TYPE_EXTERN 3
#define SYMBOL_TYPE_ENTRY 4

/* Addressing modes */
#define ADDR_MODE_IMMEDIATE 0
#define ADDR_MODE_DIRECT 1
#define ADDR_MODE_INDEX 2
#define ADDR_MODE_REGISTER 3

/* Macro keywords */
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"

/*TODO: define reserved kw here */

#endif