// TODO: implement errors using `perror()`
// TODO: implement `replace_segment()`- get file and end pos. to replace the
// segment with the new one

#include "utils.h"
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Logging messages to print and save in log file */
FILE *log_file;

int print_and_log(char *text, char *format) {
    printf("%s", text);

    if (log_file == NULL) {
        /* Log file cannot be created */
        if ((log_file = fopen(LOG_FILE_LOC, "w")) == NULL) {
            printf("[UNLOGGED:] Unable to open log file");
            return 1;
        }
        /* Log file created */
        else {
            perror("[LOGGED:] Log file created\n");
        }
    }
    fprintf(log_file, "%s", sprintf(format, text));
    return 0;
}

void exit_graceful(int exit_code, int stop) {
    printf("[EXIT:] program exits with code\n", exit_code);
    fclose(log_file);
    if (stop) {
        exit(exit_code);
    }
}
