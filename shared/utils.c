/* TODO: implement errors using `perror()`
 * TODO: implement `replace_segment()`- get file and end pos. to replace the
 * segment with the new one
 */

#include "utils.h"
#include "definitions.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Logging messages to print and save in log file */
FILE *log_file;

int print_and_log(char *text, char *format) {
    printf("%s", text);

    if (log_file == NULL) {
        /* Log file cannot be created */
        if ((log_file = fopen(LOG_F_LOC, "w")) == NULL) {
            printf("[UNLOGGED:] Unable to open log file | ");
            return 1;
        }
        /* Log file created */
        else {
            printf("[LOGGED:] Log file created\n");
        }
    }
    fprintf(log_file, format, text);
    return 0;
}

void exit_graceful(int exit_code, int stop) {
    printf("[EXIT:] program exits with code %d\n", exit_code);
    fclose(log_file);
    if (stop) {
        exit(exit_code);
    }
}

char *add_ext(const char *str1, const char *str2) {
    /* Allocate memory */
    char *result = malloc((strlen(str1) + strlen(str2)) * sizeof(char));
    if (!result) {
        printf("[UTILS:] Nenory could not be allocated\n");
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