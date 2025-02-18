#include <stdio.h>
/**
 * @brief Wrapper component to be accessed by user. Recive input here, so on...
 */

int main(int argc, char* argv[]) {
    // argc- how many args were passed
    // argv - args themselves, @0 is the name of the program (either wrapper.c or final binary name)

    /*
    What to do in wrapper:
    1. pre_assembler
    1.5 check failiures (mayvbe not here)
    2. first and second pass
    3. output file

    report throughout for debug
    */
    if (argc <= 1) {
        printf("No arguments passed\n");
    }
    else {
        printf("[DEBUG:]Arguments passed: \n");
        printf("[DEBUG:]%d\n", argc);
        for (int i = 1; i < argc; i++) {
            printf("%s\n", argv[i]);
        }

        printf("[DEBUG:]now passing to inter...\n");

    }

    return 0;
}
