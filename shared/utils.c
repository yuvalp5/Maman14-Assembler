// TODO: implement errors using `perror()`
// TODO: implement `replace_segment()`- get file and end pos. to replace the
// segment with the new one

#include "utils.h"
#include "definitions.h"
#include <stdlib.h>
#include <string.h>


int print_and_log(char *text) {
    printf("%s", text);

    if (fprintf(LOG_FILE_LOC, "%s", text) != NULL) {
        return 0;
    } else {
        perror(("[UNLOGGED:] %s", text));
        return 1;
    }
}

void exit_graceful(int exit_code, FILE *log_file, int stop) {
    print_and_log(("[EXIT:] program exits with code %d\n", exit_code));
    fclose(log_file);
    if (stop) {
        exit(exit_code);
    }
}



/**
 * ***** Keeping for future usage, we might need that for first and second pass
 * *******
 * @brief Validates a macro name against reserved words
 *
 * Ensures that macro names don't conflict with assembler instructions,
 * directives, or registers.
 * TODO: yuval - move to utils and change name to is_valid_field
 * @param name The macro name to check
 * @return 1 if the name is valid, 0 if invalid
 */
int is_valid_macro_name(const char *name) {
    /* Check if name is empty */
    if (name[0] == '\0') {
        return 0;
    }

    /* List of reserved words that can't be macro names */
    const char *reserved_words[] = {
        /* Assembly instructions */
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp",
        "bne", "jsr", "red", "prn", "rts", "stop",

        /* Assembler directives */
        "data", "string", "entry", "extern",

        /* Macro directives */
        MACRO_START_KW, MACRO_END_KW,

        /* Registers */
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",

        /* Terminator */
        NULL};

    /* Check against each reserved word */
    for (int i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(name, reserved_words[i]) == 0) {
            return 0;
        }
    }

    /* Name is valid */
    return 1;
}