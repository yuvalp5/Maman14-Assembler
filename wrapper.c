#include "wrapper.h"

int main(int argc, char* argv[]) {
    // argc- how many args were passed
    // argv - args themselves, @0 is the name of the program (either wrapper.c or final binary name)

    /*
    What to do in wrapper:
    1. pre_assembler
    1.5 check failiures (maybe not here)
    2. first and second pass
    3. output file

    report throughout for debug
    */
    if (argc <= 1) {
        printf("No arguments passed, exiting...\n");
        return 0;
    }


    printf("[DEBUG:] Arguments passed: \n");
    printf("[DEBUG:] %d\n", argc);
    for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    printf("[DEBUG:] now passing to inter...\n");
    printf("[DEBUG:] calling pre-assembler\n");
    pre_assembler(INPUT_FILE_LOC, PRE_ASSEMBLED_SAVE_LOC);

    return 0;
}
