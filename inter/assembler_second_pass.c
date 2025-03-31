/**
 * @brief Pre-assembler implementation
 * Handles macro expansion in source files
 */

 #include "pre_assembler.h"
 #include "definitions.h"
 #include "types.h"
 #include "utils.h"
 #include <ctype.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
  
 int process_line_pre_assembler(char *line, int *in_macro, char *current_macro, char *macro_content) {
     char first_word[MAX_MACRO_NAME_LEN + 1];
     char *ptr = line;
     int success = 1;
 
     /* Skip leading whitespace */
     while (*ptr && isspace((unsigned char)*ptr)) {
         ptr++;
     }
 
     /* Extract first word */
     sscanf(ptr, "%s", first_word);
 
     /* Check for macro definition start */
     if (strcmp(first_word, "mcro") == 0) {
         if (*in_macro) {
             printf("Error: Nested macro definition at line %d\n", line_number);
             success = 0;
         } else {
             if (!extract_macro_name(ptr, current_macro)) {
                 printf("Error: Invalid macro name at line %d\n", line_number);
                 success = 0;
             } else {
                 *in_macro = 1;
                 macro_content[0] = '\0';
             }
         }
     }
     /* Check for macro definition end */
     else if (strcmp(first_word, "endmcro") == 0) {
         if (!*in_macro) {
             printf("Error: endmcro without matching mcro at line %d\n", line_number);
             success = 0;
         } else {
             /* Add macro to table */
             Macro *macro = malloc(sizeof(Macro));
             if (!macro) {
                 printf("Error: Memory allocation failed at line %d\n", line_number);
                 success = 0;
             } else {
                 macro->name = strdup(current_macro);
                 macro->value = strdup(macro_content);
                 if (!macro->name || !macro->value) {
                     printf("Error: Memory allocation failed at line %d\n", line_number);
                     success = 0;
                 } else {
                     insert_item(macro_table, macro);
                 }
             }
             *in_macro = 0;
         }
     }
     /* Inside macro definition */
     else if (*in_macro) {
         /* Append line to macro content */
         if (strlen(macro_content) + strlen(line) < MAX_MACRO_CONTENT_LEN) {
             strcat(macro_content, line);
         } else {
             printf("Error: Macro content too large at line %d\n", line_number);
             success = 0;
         }
     }
     /* Check for macro usage */
     else {
         Macro *macro = get_item(macro_table, first_word);
         if (macro) {
             /* Replace macro usage with its content */
             strcpy(line, macro->value);
         }
     }
 
     return success;
 }
 
 int extract_macro_name(const char *line, char *macro_name) {
     const char *ptr = line;
     int i = 0;
 
     /* Skip "mcro" keyword */
     while (*ptr && isspace((unsigned char)*ptr)) {
         ptr++;
     }
     if (strncmp(ptr, "mcro", 4) == 0) {
         ptr += 4;
     }
 
     /* Skip whitespace after keyword */
     while (*ptr && isspace((unsigned char)*ptr)) {
         ptr++;
     }
 
     /* Extract macro name */
     while (*ptr && !isspace((unsigned char)*ptr) && i < MAX_MACRO_NAME_LEN - 1) {
         macro_name[i++] = *ptr++;
     }
     macro_name[i] = '\0';
 
     /* Validate name */
     if (i == 0 || !isalpha((unsigned char)macro_name[0])) {
         return 0;
     }
 
     return 1;
 }
 
 int is_empty_or_comment(const char *line) {
     const char *ptr = line;
 
     /* Skip leading whitespace */
     while (*ptr && isspace((unsigned char)*ptr)) {
         ptr++;
     }
 
     /* Check if line is empty or starts with ';' */
     return (*ptr == '\0' || *ptr == ';');
 }
 