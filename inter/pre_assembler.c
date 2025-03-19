
// TODO: file doc
// TODO: yuval- function docs
//  1-5: yuval
//  6-9 + pre_assembler: oren

#include "pre_assembler.h"

Macro macro_table[MAX_MACROS]; /* Array to store macros */
int macro_count = 0;           /* Counter for the number of macros */
bool is_macro = false; /* Flag indicating if we are inside a macro definition */

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
        case 0: // regular line
            fputs(line, pre_assembled);
        case 1: // new definition
            add_macro(line);
            fputs(*macro, pre_assembled);
        case 2: // existing macro
            replace_macro(pre_assembled, line);
        }
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


/**
 * @brief Reads a macro definition from a file until "mcroend" is encountered.
 *        The macro name and its content are stored in the macro table.
 *        "mcroend" is not stored in the macro's content.
 *
 * @param file Pointer to the file being read.
 * @param name The name of the macro being defined.
 *
 * @return void
 */void add_macro(FILE *file, char *name) {
    if (macro_count >= MAX_MACROS) {
        fprintf(stderr, "Error: Too many macros defined\n");
        exit(1);
    }

    Macro *current_macro = &macro_table[macro_count];
    strcpy(current_macro->name, name);
    current_macro->content[0] = '\0'; /*Initialize content*/

    char line[MAX_LINE_LEN];

    /*Read lines until "mcroend" is encountered*/
    while (fgets(line, MAX_LINE_LEN, file)) {
        if (strstr(line, "mcroend") != NULL) {
            break; /*Stop reading when "mcroend" is found*/
        }

        /*Append line to macro content safely*/
        strncat(current_macro->content, line, MAX_MACRO_CONTENT_LEN - strlen(current_macro->content) - 1);
    }

    macro_count++; /*Increment the macro count*/
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