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
