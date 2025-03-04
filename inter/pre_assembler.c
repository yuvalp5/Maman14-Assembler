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

//TODO: file doc
//TODO: yuval- function docs
// 1-5: yuval
// 6-9 + pre_assembler: oren

#include "pre_assembler.h"
#include "../shared/utils.h"
#include "../shared/types.h"

Macro macro_table[MAX_MACROS]; /* Array to store macros */
int macro_count = 0; /* Counter for the number of macros */
bool is_macro = false; /* Flag indicating if we are inside a macro definition */

// TODO: oren perform all methods as needed
void pre_assembler(FILE* open_loc, FILE* save_loc) {

    /* Run variables */
    bool is_macro;
    bool a;
    char* text;

    /* Iteration variables */
    char** fields[7][24];
    int ln;
    /* Loop + assignment of current line to fields */
    for (ln = 1; (**fields = ln_to_fields(open_loc, ln, buf, sz) != NULL); ln++) { //TODO:yuval- implement docs, not sure what buf and sz should be; if EOF encountered return NULL; we need to think of other ways to handle invalid input- maybe in later stages where actual fields are evaluated

        switch (mcr(*fields[0])) {// TODO: oren- implement function that recieves field (str pointer), if macro definition return 1, if macro already defined return 2, else return 0
        case 2: /* already defined */
            replace_macro(open_loc, *fields[0]);
        case 1: /* new definition */
            int tmp_ln;
            if (tmp_ln = add_macro(ln)) { //[x] later implement error if failed
                ln = tmp_ln;
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

void replace_macro(FILE *file, const char *line) {
    char macro_name[50]; // replace with const later
    sscanf(line, "%s", macro_name);
    for (int i=0; i<macro_count; i++){ //implement macro_count later
      if (strcmp(macro_table[i].name, macro_name) == 0) { //implelemt macro_table
        fputs(macro_table[i].content, file); // Write macro content to output if found a match
        return;
        	}
        }
        fputs(line, file); // Write the line as is if no macro is found
}

// might be modified according to  the structure of macro_table....
// is_macro bool will be handled in the pre_assembler func
//TODO: yuval- return new line number value (after macroend) on success to add; 0 if table is full
//TODO: yuval- read until "MACROEND", then delete it
// note- maybe return line where macroend is? what if macroend is right before EOF?
void add_macro(const char* line) {
    if (macro_count >=MAX_MACROS){
      printf("Too many macros in the program\n");
      return;
      }
      // Concatenate the new line to the last macro content
    strncat(macro_table[macro_count - 1].content, line, MAX_MACRO_CONTENT_LEN - strlen(macro_table[macro_count - 1].content) - 1);

}

//9:
/**
 * @brief saves the given text to the output file
 * @param text the text to save
 * @param save_loc the location to save the file to (should be in ../io/)
 * @return 0 if the file was saved successfully; 1 if file can't be opened in `w` mode; 2 if writing failed; 3 if closing file failed
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