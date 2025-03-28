#include "wrapper.h"
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
    char *src_name, *pre_assembled_name, *object_name, *externals_name,
        *entries_name;
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
        char *base_name = argv[i];
        src_name = strcat(base_name, SRC_F_EXT);
        pre_assembled_name = strcat(base_name, PAS_F_EXT);
        object_name = strcat(base_name, OBJ_F_EXT);
        externals_name = strcat(base_name, EXT_F_EXT);
        entries_name = strcat(base_name, ENT_F_EXT);

        /* Pre-assembler */
        if (pre_assembler(src_name, pre_assembled_name)) {
            printf("[WRAPPER:] Pre-assembler completed.\n");
        } else {
            printf("[WRAPPER:] Pre-assembler failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }

        /* First pass */
        if (first_pass(src_name)) {
            printf("[WRAPPER:] First pass completed.\n");
        } else {
            printf("[WRAPPER:] First pass failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }

        /* Second pass */
        if (second_pass(src_name)) {
            printf("[WRAPPER:] Second pass completed.\n");
        } else {
            printf("[WRAPPER:] Second pass failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }
        /*TODO: report whether externals and entries were produced? */
        /* TODO is this everything? Check */
    }
    printf("[WRAPPER:] All operations completed successfully.\n");

    return 0;
}
