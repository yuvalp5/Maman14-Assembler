/**
 * @brief Wrapper component to be accessed by user. Main entry point used to
 * recive input and pass it on through all stages as well as to print the output
 * to the user. Included are also definitions for use in the project.
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H 1

/* Project-wide definitions */
#define LOG_FILE_LOC "./io/log.txt"
#define MAX_MACRO_NAME_LEN 31
#define MAX_MACRO_CONTENT_LEN 1000
#define MAX_MACROS 50

/* Constants */
#define MAX_LINE_LEN 80
#define MAX_SYMBOL_LEN 31
#define MAX_SYMBOLS 256

/* File extensions */
#define SRC_F_EXT ".as"
#define PAS_F_EXT ".am"
#define OBJ_F_EXT ".ob"
#define EXT_F_EXT ".ext"
#define ENT_F_EXT ".ent"

/* Macro keywords */
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"

#endif