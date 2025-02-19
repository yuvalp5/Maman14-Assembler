/*
algorithm for pre-assembler:
Read the next line from the source file. If the end of the macro is reached, proceed to step 9 (end).
If the first field is the name of a macro appearing in the macro table (e.g., a_mc), replace the macro name with its content, copying all the corresponding lines from the macro table. Then, return to step 1.
If the first field is "mcro" (macro definition start), proceed to step 6. If not, proceed to step 4.
Print the line as is.
(During macro definition) Insert the line into the macro table under the macro name (e.g., a_mc).
Read the next line from the source file. If the end of the macro is reached, proceed to step 9 (end).
If the first field is "mcroend" (macro definition end), insert the line into the macro table. Then, return to step 1.
If the first field is "mcro", remove the previous macro directive from the source file. If not, return to step 6.
End: Save the expanded source file.
 */

 // 1-5: yuval
// 6-9 + pre_assembler: oren

#include "pre_assembler.h"

//0, 4, 6-8, may be broken up further later
void pre_assembler(int open_loc, int save_loc) {
    // TODO: perform all methods as needed
    // TODO: implement in-between parts
    is_macro = false;
    int ln = 1;
    char** fields[7][24];

    while ((**fields = read_ln_num(text, ln)) != NULL) {}
}

// 1:
void read_ln_num(int text, int ln_num) {
    // TODO: read line number `ln_num` until \n and return *char[] of all fields separated, if reached eof return NULL
    // deleted because needs to be redone to fit method-based run. sorry
}

//2:
void replace_macro(int text) {
    // TODO: replace given text until \n with EXISTING macro text
}


//3-5: (I may be wrong)
void add_macro(int text) {
    // TODO: implement structure "macro table" (will be done later)
    // TODO: create new entry in structure "macro table"
    // 4:
    is_macro = true;
}

//9:
void save_file(int text) {
    FILE* output_file = fopen("output.txt", "w");
    fprintf(output_file, "%s", text);
    fclose(output_file);
}