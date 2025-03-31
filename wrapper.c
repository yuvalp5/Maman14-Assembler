/**
 * @brief Entry point for program. Handles CLI and passes input to functions.
 * file names to be proccessed are to be provided as arguments without file
 * extensions
 */

#include "assembler_first_pass.h"
#include "assembler_second_pass.h"
#include "definitions.h"
#include "pre_assembler.h"
#include "utils.h"
#include <stdio.h>

/**
 * @brief Main function for the assembler
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 if successful; 1 otherwise
 */
int main(int argc, char *argv[]) {

    /* Iterator */
    int i;

    /* Check passed args */
    if (argc < 2) {
        printf("[WRAPPER:] No arguments provided. File names expected as "
               "arguments.\n");
        printf("[WRAPPER:] Exiting...\n");
        return 1;
    }

    /* Iterate over args */
    for (i = 1; i < argc; i++) {

        /* Pre-assembler */
        if (pre_assembler(argv[i]) == 0) {
            printf("[WRAPPER:] Pre-assembler completed.\n");
        } else {
            printf("[WRAPPER:] Pre-assembler failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }

        /* First pass */
        if (first_pass(argv[i]) == 0) {
            printf("[WRAPPER:] First pass completed.\n");
        } else {
            printf("[WRAPPER:] First pass failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }

        /* Second pass */
        if (second_pass(argv[i]) == 0) {
            printf("[WRAPPER:] Second pass completed.\n");
        } else {
            printf("[WRAPPER:] Second pass failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }
    }
    printf("[WRAPPER:] All operations completed successfully.\n");

    return 0;
}
