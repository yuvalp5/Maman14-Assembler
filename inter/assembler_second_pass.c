#include "assembler_second_pass.h"
void second_pass() {
    // 2 skip if label
    // 7 keep track of all errors - dont stop until end
}

// 1
char *ln_to_fields(FILE *file, int ln_num, char *buffer, int size); // from
                                                                    // utils

// 3
void is_data_or_string_or_extern(int field);

// 4+5
void add_entry_property_to_table(int field);

// 6
void complete_binary(int field);

// 8
void create_ouput_files(int field);
