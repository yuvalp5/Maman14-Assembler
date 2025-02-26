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

    /* Run variables */
    bool is_macro;
    bool a;
    char* text;

    /* Iteration variables */
    char** fields[7][24]; // TODO maybe implement as type?
    int ln;
    for (ln = 1; (**fields = ln_to_fields(ln) != NULL); ln++) {

        if (is_macro) {
            if (*fields[0] == "mcroend") {
                is_macro = false;
                continue;
            }
            if (fields[0] == "mcro") {
                continue;
            }
            add_macro(ln);
        }
        else {
            if (fields[0] == "mcro") {
                add_macro(ln);
                continue;
            }
            replace_macro(ln);
        }
    }

    /* Save at EOF recieved */
    switch (save_file(text, save_loc))
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    }

}

// 1:


//2:
void replace_macro(int text) {
    // TODO: replace given text until \n with EXISTING macro text
    //delete macro name
    //replace with relevant values
}


//3-5: (I may be wrong)
void add_macro(int text) {
    // TODO: implement structure "macro table" (will be done later)
    // TODO: create new entry in structure "macro table"
    // read the first field of each line
    //if its a macro, add it to the macro table
    //else, continue
    // 4:
    is_macro = true; // not sure if needed
}

//9:
/**
 * @brief saves the given text to the output file
 * @param text the text to save
 * @param save_loc the location to save the file to (should be in ../io/)
 *@return 0 if the file was saved successfully; 1 if file can't be opened in `w` mode; 2 if writing failed, 3 if closing file failed
 */
 /* TODO: Move this to shared/utils */
int save_file(int text, char* save_loc[]) {
  //TODO: replace text param with pointer to file
    /* Create FILE type */
    FILE* output_file = fopen(*save_loc, "w");

    /* failiure to create file in `w` mode */
    if (!output_file) {
        return 1;
    }

    /* failiure to write to file */
    if (!fprintf(output_file, "%s", text))
        return 2;

    /* failiure to close file gracefully */
    if (!fclose(output_file))
        return 3;

    /* success */
    return 0;
}