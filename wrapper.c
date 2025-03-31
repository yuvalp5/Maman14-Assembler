/**
 * @brief Entry point for program. Handles CLI and passes input to functions.
 * file names to be proccessed are to be provided as arguments without file
 * extensions
 */

#include "inter/assembler_first_pass.h"
#include "inter/assembler_second_pass.h"
#include "definitions.h"
#include "inter/pre_assembler.h"
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
    /*char *src_name, *pre_assembled_name,  *externals_name,
        *entry_name;*/
    char *object_name;
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
        /*src_name = add_ext(argv[i], SRC_F_EXT);*/ 
        /*pre_assembled_name = add_ext(argv[i], PAS_F_EXT);*/
        object_name = add_ext(argv[i], OBJ_F_EXT);
        /*externals_name = add_ext(argv[i], EXT_F_EXT);*/
        /*entry_name = add_ext(argv[i], ENT_F_EXT);*/

        /* Pre-assembler */
        if (pre_assembler(argv[i])) {
            printf("[WRAPPER:] Pre-assembler completed.\n");
        } else {
            printf("[WRAPPER:] Pre-assembler failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }

        /* First pass */
        if (first_pass(argv[i])) {
            printf("[WRAPPER:] First pass completed.\n");
        } else {
            printf("[WRAPPER:] First pass failed.\n");
            printf("[WRAPPER:] Exiting...\n");
            return 1;
        }

        /* Second pass */
        if (second_pass(object_name)) {
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
