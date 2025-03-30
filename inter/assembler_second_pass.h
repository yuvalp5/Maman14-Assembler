/* Second pass header for the assembler
 * Handles the generation of machine code and output files
 */

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H 1

/**
 * @brief Master function for second pass
 * @param src_file Path of file to perform pass on
 * @param dest_file Path of file save
 * @return 0 if successful; 1 otherwise
 */
int second_pass(const char *src_file, const char *dest_file);

#endif
