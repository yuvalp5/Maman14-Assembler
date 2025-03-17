/*
algorithm for pre-assembler:
Read the next line from the source file. If the end of the macro is reached,
proceed to step 9 (end). If the first field is the name of a macro appearing in
the macro table (e.g., a_mc), replace the macro name with its content, copying
all the corresponding lines from the macro table. Then, return to step 1. If the
first field is "mcro" (macro definition start), proceed to step 6. If not,
proceed to step 4. Print the line as is. (During macro definition) Insert the
line into the macro table under the macro name (e.g., a_mc). Read the next line
from the source file. If the end of the macro is reached, proceed to step 9
(end). If the first field is "mcroend" (macro definition end), insert the line
into the macro table. Then, return to step 1. If the first field is "mcro",
remove the previous macro directive from the source file. If not, return to
step 6. End: Save the expanded source file.
 */

// TODO: file doc
// TODO: yuval- function docs
//  1-5: yuval
//  6-9 + pre_assembler: oren

#include "pre_assembler.h"

Macro macro_table[MAX_MACROS]; /* Array to store macros */
int macro_count = 0;           /* Counter for the number of macros */
bool is_macro = false; /* Flag indicating if we are inside a macro definition */

// TODO: oren perform all methods as needed
void pre_assembler(char *src, char *dest) {
    /* Files to work on */
    FILE *user_input, *pre_assembled;
    user_input = fopen(src, "r");
    pre_assembled = fopen(dest, "w+");

    /* Run variables */
    bool is_macro;
    char *text, *macro;

    /* Iteration variables */
    char *line;
    char **fields[7][24];

    /* Loop + assign fields and advance file */
    while ((line = fgets(fields, MAX_LINE_LEN, pre_assembled)) &&
           (**fields = line_to_fields(line) != NULL)) {

        switch (macro_definition(*fields[0])) {
        case 0: // regular line - todo- do we need to check syntax errors?
            fputs(line, pre_assembled);
        case 1: // new definition
            add_macro(line);
            fputs(*macro, pre_assembled);
        case 2: // existing macro
            replace_macro(pre_assembled, line);
        }

        // TODO: oren- check if fgets returned NULL because eof or because line
        // too long- DONE
        /* Save at EOF recieved - make sure  no errors encountered */
        if (!fclose(pre_assembled)) {
            perror("Failed to close file");
        }
    }
}

int macro_definition(char *field) {
    /* New definition */
    if (!strcmp(field, "mcro")) {
        return 1;
    }
    /* Call to existing */
    int i;
    for (i = 0; i < macro_count; i++) {
        if (!strcmp(macro_table[i].name, field)) {
            return 1;
        }
    }
    /* Not macro */
    return 0;
}

void replace_macro(FILE *file, const char *line) {
    char macro_name[50]; // replace with const later
    sscanf(line, "%s", macro_name);
    for (int i = 0; i < macro_count; i++) { // implement macro_count later
        if (strcmp(macro_table[i].name, macro_name) ==
            0) { // implelemt macro_table
            fputs(macro_table[i].content,
                  file); // Write macro content to output if found a match
            return;
        }
    }
    fputs(line, file); // Write the line as is if no macro is found
}

// might be modified according to  the structure of macro_table....
// is_macro bool will be handled in the pre_assembler func
// TODO: yuval- read lines until "macroend" recieved then delete it
// note- maybe return line where macroend is? what if macroend is right before
// EOF?
int add_macro(FILE *file) {
    if (macro_count >= MAX_MACROS) {
        printf("Too many macros in the program\n");
        return;
    }
    // Concatenate the new line to the last macro content
    strncat(macro_table[macro_count - 1].content, line,
            MAX_MACRO_CONTENT_LEN -
                strlen(macro_table[macro_count - 1].content) - 1);
}

/**
 * @brief get macro by name from table
 * @param macro_name the name of the macro
 * @return the content of the macro, NULL if not found
 */
char *get_macro(const char *macro_name) { // TODO: oren- check
    for (int i = 0; i < macro_count; i++) {
        if (strcmp(macro_table[i].name, macro_name) == 0) {
            return macro_table[i].content;
        }
    }
    return NULL;
}