#include "assembler_first_pass.h"

void first_pass(*char src) {
    // 1
    int IC = 100;
    int DC = 0;
    int is_symbol = 0;
    char line[MAX_LINE_LEN];
    char first_word[MAX_LINE_LEN];
    FILE *input_pre_assembled;

    input_pre_assembled = fopen(src, "r");
    while (fgets(line, MAX_LINE_LEN, input_pre_assembled) !=NULL) {
       first_word = split_line_to_fields(line); // create in utils
       if (is_symbol(first_word)) {
           is_symbol = 1;
       }
       if (is_storage(first_word)) {
           add_symbol_to_table(symbol_name, DC);
           //raise error if the symbol is already in the table
           // stage 7
       }
       if(!(is_extern || is_entry)){
           //stage 11
           }
       if (is_extern) {
           //to stage 11
           add_symbol_to_table
       }
       if (is_entry) {
           // to stage 2
       }





    // 4 new symbol definition
    // 16  -update IC+L -> IC and go to to stage 2
    // 17 keep track of all errors - dont stop until end
    // 18 - save final values if IC, DC as ICF, DCF
    // 20 - move to second pass
}

// 3- almost same as macro check from pre_assembler.c
// todo- maybe same check as from pre_assembler but more return options
int is_symbol(int field) {
    /* New definition */
    if (!strcmp(field, "mcro")) {
        return 1;
    }
    /* Call to existing */
    int i;
    for (i = 0; i < macro_table.len; i++) { /* I honestly hate this */
        if (!strcmp(macro_table.items[i].name, field)) {
            return 1;
        }
    }
    /* Not symbol */
    return 0;
}

// 5
int is_storage(int field) { // todo yuval- movbe this check into master method.
                            // the check is just the if statement
    if (!strcmp(field, ".data") || !strcmp(field, ".string")) {
        return 1;
    }
    return 0;
}

// 6+7
int add_symbol_to_table(
    int name, int val); // todo oren- implement later when types are done

// 8+9+10
void is_extern_or_entry(
    int field) { // todo yuval- movbe this check into master method.
    // the check is just the if statement
    if (!strcmp(field, ".extern") || !strcmp(field, ".entry")) {
        return 1;
    }
    return 0;
};

// 11 - if everything else is false
void add_code_to_table(int field);

// 12
void search_command_in_table(int field);

// 13
void calc_num_of_words(int field);

// 14
void word_to_binary(int field);

// 15
void save_values_with_binary(int field); // TODO: need to understand WTF this is

// 19
void add_lcf_to_data(int field);
