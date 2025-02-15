/* algorithm for pre-assembler:
Read the next line from the source file. If the end of the macro is reached, proceed to step 9 (end).
If the first field is the name of a macro appearing in the macro table (e.g., a_mc), replace the macro name with its content, copying all the corresponding lines from the macro table. Then, return to step 1.
If the first field is "mcro" (macro definition start), proceed to step 6. If not, proceed to step 4.
Print the line as is.
(During macro definition) Insert the line into the macro table under the macro name (e.g., a_mc).
Read the next line from the source file. If the end of the macro is reached, proceed to step 9 (end).
If the first field is "mcroend" (macro definition end), insert the line into the macro table. Then, return to step 1.
If the first field is "mcro", remove the previous macro directive from the source file. If not, return to step 6.
End: Save the expanded source file.
 */
