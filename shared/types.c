#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global counter definitions */
int IC = 100;         /* Instruction Counter starts at 100 */
int DC = 0;           /* Data Counter starts at 0 */
int error_count = 0;  /* Number of errors encountered */
int LINE_NUMBER = 0;  /* Current line being processed */
int MEMORY_SIZE = 0;  /* Total memory size (IC + DC) */
int ICF = 0;          /* Final IC value */
int DCF = 0;          /* Final DC value */
int symbol_count = 0; /* Number of symbols in symbol table */

/* Label table instance */
Table label_table = {0, NULL};
/* Macro table instance */
Table macro_table = {0, NULL};
/* Symbol table instance */
Table symbol_table[MAX_SYMBOLS] = {};
