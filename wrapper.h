/**
 * @brief Wrapper component to be accessed by user. Main entry point used to
 * recive input and pass it on through all stages as well as to print the output
 * to the user.
 */

/* Standard file extenstions for io files */
#define SOURCE_FILE_EXT ".as"
#define PRE_ASSEMBLED_FILE_EXT ".am"
#define OBJECT_FILE_EXT ".ob"
#define OUTPUT_FILE_EXT ".out"
#define ENTRY_FILE_EXT ".ent"
#define EXTERN_FILE_EXT ".ext"
#define PRE_ASSEMBLED_SAVE_LOC "./io/pre_assembler.txt"

#include "inter/assembler_first_pass.h"
#include "inter/assembler_second_pass.h"
#include "inter/pre_assembler.h"

#include <stdio.h>
