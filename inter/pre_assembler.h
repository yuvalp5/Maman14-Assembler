/**
 * @brief Pre-assembler implementation as per P. 40
 */

#include "../shared/types.h"
#include "../shared/utils.h"

/**
 * @brief Pre-assembler method to perform algorythm
 *
 * @param open_loc File to open
 * @param save_loc File to save to
 */
void pre_assembler(char *src, char *dest); // todo int with success/fail output
int macro_definition(char *field);
void replace_macro(FILE *file, const char *line);
int add_macro(FILE *file);
char *get_macro(const char *macro_name);

bool is_macro;
