#include "utils.h"
#include "definitions.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exit_graceful(int exit_code, int stop) {
    printf("[UTILS:] program exits with code %d\n", exit_code);
    if (stop) {
        exit(exit_code);
    }
}

/*int is_reserved_kw(char *keyword) {
    int i;
    for (i = 0; RESERVED_KW[i] != NULL; i++) {
        if (strcmp(keyword, RESERVED_KW[i]) == 0) {
            return 1;
        }
    }
    return 0;
}*/

char *add_ext(const char *str1, const char *str2) {
    /* Allocate memory */
    char *result = malloc((strlen(str1) + strlen(str2)) * sizeof(char));
    if (!result) {
        printf("[UTILS:] Memory could not be allocated\n");
        return NULL;
    }

    /* Concatenate */
    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

int extract_macro_name(const char *line, char *macro_name) {
    const char *ptr = line;
    int i;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Skip "mcro" keyword */
    if (strncmp(ptr, MACRO_START_KW, strlen(MACRO_START_KW)) == 0) {
        ptr += strlen(MACRO_START_KW);
    } else {
        return 0; /* Not a macro definition line */
    }

    /* Skip whitespace after keyword */
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    /* Extract the name */
    i = 0;
    while (*ptr && !isspace((unsigned char)*ptr) &&
           i < MAX_MACRO_NAME_LEN - 1) {
        macro_name[i++] = *ptr++;
    }
    macro_name[i] = '\0';

    /* Validate name is not empty */
    return (i > 0);
}

int is_reserved_word(const char *name) {
    int i;
    /* List of reserved words that can't be macro names */
    /* TODO use RESERVED_KW */
    const char *reserved_words[] = RESERVED_KW;

    /* Check if name is empty */
    if (name[0] == '\0') {
        return 0;
    }

    /* Check against each reserved word */
    for (i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(name, reserved_words[i]) == 0) {
            return 0;
        }
    }

    /* Name is valid */
    return 1;
}