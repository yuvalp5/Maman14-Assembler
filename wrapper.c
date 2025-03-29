#include "wrapper.h"
#include "assembler_first_pass.h"
#include "assembler_second_pass.h"
#include "definitions.h"
#include "pre_assembler.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Main function for the assembler
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 if successful; 1 otherwise
 */
int main(int argc, char *argv[]) {
    /* File names */
    char *base_name, *src_name, *pre_assembled_name, *object_name,
        *externals_name, *entry_name;
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
        /* Assign file names */
        base_name = argv[i], src_name = strcat(base_name, ".as");
        pre_assembled_name = strcat(base_name, ".am");
        object_name = strcat(base_name, ".ob");
        externals_name = strcat(base_name, ".external");
        entry_name = strcat(base_name, ".entry");

        /* Pre-assembler */
        if (pre_assembler(src_name, pre_assembled_name)) {
            printf("[WRAPPER:] Pre-assembler completed.\n");
        } else {
            printf("[WRAPPER:] Pre-assembler failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }

        /* First pass */
        if (first_pass(pre_assembled_name, object_name)) {
            printf("[WRAPPER:] First pass completed.\n");
        } else {
            printf("[WRAPPER:] First pass failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }

        /* Second pass */
        if (second_pass(object_name, object_name)) {
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
