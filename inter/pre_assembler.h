/**
 * @brief Pre-assembler implementation as per P. 40
 */

#include <stdio.h>
#include <stdlib.h>
#include "../shared/types.h"

 /**
  * @brief Pre-assembler method to perform algorythm, rsulting file is saved (where?)
  */
void pre_assembler(int text, int save_loc);

// these should maybe be internal (only in .c)
void pre_assembler(int text, int save_loc);
char** ln_to_fields(FILE *file, int ln_num);
void replace_macro(FILE *file, const char *line);
void add_macro(const char *line);
int save_file(int text, char* save_loc[]);

bool is_macro;
