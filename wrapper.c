/**
 * @brief Entry point for program. Handles CLI and passes input to functions.
 */

#include "wrapper.h"
#include "inter/assembler_first_pass.h"
#include "inter/assembler_second_pass.h"
#include "inter/pre_assembler.h"
#include "shared/definitions.h"
#include "shared/utils.h"

#include <stdio.h>

/**
 * @brief main() function
 * @param argc number of arguments passed
 * @param argv arguments pointers
 * @return int
 */
int main(const int argc, char *argv[]) {

    if (argc <= 1) {
        print_and_log("[WRAPPER:] No arguments passed, exiting...\n");
        return 0;
    }

    print_and_log("[WRAPPER:] Arguments passed: \t");
    for (int i = 0; i < argc; i++) {
        print_and_log(("%d - %s\n", i, argv[i]));
    }

    print_and_log("[WRAPPER:] calling pre-assembler\n");

    /* Iterate command line args */
    int i;
    char *src_name, *pre_assembled_name, *object_name, *output_name;

    /* Pre-assembler */
    for (i = 1; i < argc; i++,

        src_name = strcat(argv[i], SOURCE_FILE_EXT),
        pre_assembled_name = strcat(argv[i], PRE_ASSEMBLED_FILE_EXT)) {

        print_and_log("[WRAPPER:] full file name passed: %d\n");
        pre_assembler(src_name, pre_assembled_name);
    }

    /* First + second pass */
    print_and_log("[WRAPPER:] calling first + second pass\n");
    for (i = 1; i < argc; i++,

        pre_assembled_name = strcat(argv[i], PRE_ASSEMBLED_FILE_EXT),
        object_name = strcat(argv[i], OBJECT_FILE_EXT)) {

        print_and_log(
            ("[WRAPPER:] full file name passed: %d\n", pre_assembled_name));
        // assemble(src_name, pre_assembled_name, object_name);
        //  TODO IMPLEMENT assemble() FUNCTION TO CALL BOTH PASSES
    }

    exit_graceful(0, 0);
    return 0;
}
