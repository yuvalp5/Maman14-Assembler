/**
 * @brief Entry point for program. Handles CLI and passes input to functions.
 */

#include "wrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Main function for the assembler
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 if successful, 1 otherwise
 */
int main(int argc, char *argv[]) {
    char *src_name, *pre_assembled_name;
    int i;

    /* Check if any files were provided */
    if (argc < 2) {
        printf("Usage: %s file1 [file2 ...]\n", argv[0]);
        return 1;
    }

    /* Process each file */
    for (i = 1; i < argc; i++) {
        src_name = argv[i];
        pre_assembled_name = (char *)malloc(strlen(src_name) + 4);
        if (!pre_assembled_name) {
            printf("Error: Memory allocation failed\n");
            return 1;
        }

        /* Create filenames */
        strcpy(pre_assembled_name, src_name);
        strcat(pre_assembled_name, ".am");

        /* Run pre-assembler */
        if (pre_assembler(src_name, pre_assembled_name) != 0) {
            printf("Error: Pre-assembler failed for %s\n", src_name);
            free(pre_assembled_name);
            continue;
        }

        /* Run first pass */
        first_pass(src_name);

        /* Run second pass */
        second_pass(src_name);

        /* Cleanup */
        free(pre_assembled_name);
    }

    return 0;
}
