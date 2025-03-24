#include <stdio.h>
#include <stdlib.h>
#include "inter/assembler_first_pass.h"

int main(int argc, char *argv[]) {
    const char *filename;
    
    /* Check if a filename was provided */
    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = "test.am";  /* Default test file */
    }
    
    printf("Starting first pass on file: %s\n", filename);
    
    /* Run first pass */
    first_pass(filename);
    
    printf("First pass completed.\n");
    printf("Total errors: %d\n", error_count);
    
    return 0;
} 