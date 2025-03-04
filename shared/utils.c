//TODO:idea for file handling workflow- implement replace_line function which will get a file and position and text, and will replace that line with given text
//TODO: think over methods to work on a file

#include "utils.h"
//split the line to fields with strtok
    //return the pointer to the first field
char* ln_to_fields(FILE* file, int ln_num, char* buffer, int size) {
    if (file == NULL || ln_num < 1 || buffer == NULL) {
        return NULL;  // Handle invalid input
    }

    rewind(file);  // Move to the beginning of the file
    int current_line = 1;

    while (fgets(buffer, size, file) != NULL) {  // read file to line
        if (current_line == ln_num) {
            char* field = strtok(buffer, " \t");  // Extract the first field
            return field;  // Return the pointer to the first field
        }
        current_line++;
    }
}
