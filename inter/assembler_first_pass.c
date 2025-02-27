#include <stdbool.h>
#include "assembler_first_pass.h"
void first_pass(){
  //1
  int IC = 100;
  int DC = 0;
  bool is_symbol = false;
  //4 new symbol definition
  //16  -update IC+L -> IC and go to to stage 2
  //17 keep track of all errors - dont stop until end
  //18 - save final values if IC, DC as ICF, DCF
  //20 - move to second pass


}
//2 - from utils
char* ln_to_fields(FILE *file, int ln_num, char *buffer, int size);

//3
void is_symbol(int field);

//5
void is_storage(int field);

//6+7
void add_symbol_to_table(int field1, int field2);


//8+9+10
void is_extern_or_entry(int field);

//11 - if everything else is "false"
void add_code_to_table(int field);

//12
void search_command_in_table(int field);

//13
void calc_num_of_words(int field);

//14
void word_to_binary(int field);

//15
void save_values_with_binary(int field); //TODO: need to understand WTF this is

//19
void add_lcf_to_data(int field);










