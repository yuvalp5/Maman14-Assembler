// TODO: implement errors using `perror()`
// TODO: implement `replace_segment()`- get file and end pos. to replace the
// segment with the new one

#include "utils.h"
#include "definitions.h"
#include <stdlib.h>
#include <string.h>

/* Takes a line  and splits it to fields based on delimiters*/
// char *line_to_fields(char *text) {
//     // sorry, file handling and navigation is not needed because ive made
//     // changes to how the file is handled- you get the current line until \n
//     // ready for use
//     //  TODO: yuval- field buffer can be other things like colon/comma/etc,
//     and
//     //  the spec doesnt require whitespaces. example- `mov ax,bx` are 3
//     fields.
//     //        read page 41 and determine which checks to perform for each
//     field char *field = strtok(text, " \t"); // Extract the first field
//     return field;                      // Return the pointer to the first
//     field
// }

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
