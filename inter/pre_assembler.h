/**
 * @brief Pre-assembler implementation as per P. 40
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

 /**
  * @brief Pre-assembler method to perform algorythm, rsulting file is saved (where?)
  */
void pre_assembler(int text, int save_loc);

// these should maybe be internal (only in .c)
void pre_assembler(int text, int save_loc);
void read_next_line(int text);
void replace_macro(int text);
void add_macro(int text);
void save_file(int text);

bool is_macro;
