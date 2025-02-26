/**
 * @brief Pre-assembler implementation as per P. 40
 */

#include <stdio.h>
#include <stdlib.h>
#include "../shared/types.h"
#define MAX_LINE_LEN 256
#define MACRO_START_KW "mcro"
#define MACRO_END_KW "mcroend"
 /**
  * @brief Pre-assembler method to perform algorythm, rsulting file is saved (where?)
  */
void pre_assembler(int text, int save_loc);

// these should maybe be internal (only in .c)
void pre_assembler(int text, int save_loc);
char** ln_to_fields(FILE *file, int ln_num);
void replace_macro(int text);
void add_macro(int text);
int save_file(int text, char* save_loc[]);

bool is_macro;
