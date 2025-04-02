/**
 * @brief First pass implementation. Handles symbol table creation, instruction processing, and data storage.
 */

#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H 1

/* Memory for storing machine code - declared globally for second pass access */
extern int *code_memory;

/**
 * @brief Master function for first pass
 * @param file_basename Base name of the source assembly file (without
 * extension)
 * @return 0 on success, non-zero on failure
 */
int first_pass(const char *file_basename);

/* Helper functions */
/**
 * @brief Adds a symbol to the symbol table
 * @param symbol_name The name of the symbol to add
 * @param value The memory address or value for the symbol
 * @param type The symbol type (code, data, extern, entry)
 * @return 0 on success, non-zero on failure
 */
int handle_symbol_addition(const char *symbol_name, int value, int type);

/**
 * @brief Initializes the code memory segment
 * @return 0 on success, non-zero on failure
 */
int initialize_code_memory();

/* Symbol and label handling */
/**
 * @brief Checks if a field is a symbol definition
 * @param field The field to check
 * @return 1 if the field is a symbol, 0 otherwise
 */
int is_symbol(char *field);

/**
 * @brief Checks if a field is a storage directive (.data, .string)
 * @param field The field to check
 * @return The storage directive code if found, 0 otherwise
 */
int is_storage(char *field);

/**
 * @brief Checks if a field is an extern or entry directive
 * @param field The field to check
 * @return The directive code if found, 0 otherwise
 */
int is_extern_or_entry(char *field);

/**
 * @brief Checks if a word is a reserved word in the assembly language
 * @param word The word to check
 * @return 1 if reserved, 0 otherwise
 */
int is_reserved_word(const char *word);

/* Symbol table functions */
/**
 * @brief Adds a symbol to the symbol table
 * @param name The name of the symbol
 * @param value The value/address of the symbol
 * @param type The type of the symbol
 * @return 0 on success, non-zero on failure
 */
int add_symbol_to_table(const char *name, int value, int type);

/**
 * @brief Searches for a symbol in the symbol table
 * @param name The name of the symbol to find
 * @return The index of the symbol in the table, or -1 if not found
 */
int find_symbol_in_table(const char *name);

/**
 * @brief Gets the value of a symbol
 * @param name The name of the symbol
 * @return The value of the symbol, or -1 if not found
 */
int get_symbol_value(const char *name);

/**
 * @brief Gets the type of a symbol
 * @param name The name of the symbol
 * @return The type of the symbol, or -1 if not found
 */
int get_symbol_type(const char *name);

/**
 * @brief Adds the current instruction counter to all data symbols
 * @param offset The offset to add (typically the final instruction counter)
 */
void add_lcf_to_data(int offset);

/**
 * @brief Prints the contents of the symbol table
 */
void print_symbol_table(void);

/* Instruction processing */
/**
 * @brief Searches for a command in the commands table
 * @param command The command to search for
 * @return The command code if found, -1 otherwise
 */
int search_command_in_table(char *command);

/**
 * @brief Calculates the number of words needed for an instruction
 * @param instruction The instruction name
 * @param operand1 The first operand (or NULL)
 * @param operand2 The second operand (or NULL)
 * @return The number of words needed
 */
int calc_num_of_words(char *instruction, char *operand1, char *operand2);

/**
 * @brief Determines the addressing mode of an operand
 * @param operand The operand to analyze
 * @return The addressing mode code
 */
int get_addressing_mode(char *operand);

/**
 * @brief Gets the number of extra words needed for an operand
 * @param addressing_mode The addressing mode of the operand
 * @return The number of extra words needed
 */
int get_operand_words(int addressing_mode);

/**
 * @brief Converts an instruction to its binary representation
 * @param instruction_index The index of the instruction in the command table
 * @param operand1_mode The addressing mode of the first operand
 * @param operand2_mode The addressing mode of the second operand
 * @param code Pointer to where the binary code will be stored
 * @param code_index Pointer to the current index in the code array
 */
void word_to_binary(int instruction_index, int operand1_mode, int operand2_mode,
                    unsigned int *code, int *code_index);

/**
 * @brief Saves encoded binary values to memory
 * @param code The binary code array
 * @param word_count The number of words to save
 * @param memory The memory array to save into
 * @param current_ic The current instruction counter value
 */
void save_values_with_binary(unsigned int *code, int word_count, int *memory,
                             int current_ic);

/* Data storage handling */
/**
 * @brief Encodes data storage directives (.data, .string) into memory
 * @param directive The directive type (.data or .string)
 * @param operands The operands of the directive
 * @param data_memory The data memory segment to store into
 * @return The number of words stored, or -1 on error
 */
int encode_data_storage(const char *directive, char *operands,
                        int *data_memory);

/* Operand validation */
/**
 * @brief Validates an operand against instruction requirements
 * @param operand The operand to validate
 * @param instruction_index The index of the instruction in the command table
 * @param operand_num The operand number (1 or 2)
 * @param addressing_mode The addressing mode of the operand
 * @return 1 if valid, 0 otherwise
 */
int validate_operand(char *operand, int instruction_index, int operand_num,
                     int addressing_mode);

#endif