#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _insert_item(Table *instance, void *item);
void *_safe_malloc(const int size, const char *table_name);

/* Global instances */
Table _tbl1 = {0, NULL};
Table _tbl2 = {0, NULL};
Table _tbl3 = {0, NULL};
Table *label_table = &_tbl1;
Table *macro_table = &_tbl2;
Table *symbol_table = &_tbl3;

/* Global variables */
int DC = 0;           /* Data counter initialized to 0 */
int IC = 100;         /* Instruction counter initialized to 100 */
int symbol_count = 0; /* Number of symbols in the symbol table */
int error_count = 0;  /* Number of errors */
int line_number = 0;  /* Line number */
int ICF, DCF;         /* Final IC and DC values */
int L;                /* Number of words in the instruction */

int _insert_item(Table *instance, void *item) {
    /* Reallocate memory with 1 slot increase every time */
    instance->content = (void **)realloc(
        instance->content, (instance->count + 1) * sizeof(void *));

    /* Check if realloc succeeded */
    if (!instance->content) {
        printf("[TYPES:] Table memory allocation failed\n");
        return 1;
    }

    /* Insert the new item */
    instance->content[instance->count] = item;
    instance->count++;

    return 0;
}

void *_safe_malloc(const int size, const char *table_name) {
    void *ptr = malloc(size);
    if (!ptr) {
        printf("[TYPES:] Memory allocation failed for table: %s\n", table_name);
    }
    return ptr;
}

int insert_macro(const char *name, const char *value) {
    /* Allocate memory for new macro structure */
    Macro *macro = (Macro *)_safe_malloc(sizeof(Macro), "macro_table");
    if (!macro)
        return 1;

    macro->name = name;
    macro->value = strdup(value); /* Use strdup to simplify allocation */

    /* Check if strdup succeeded */
    if (!macro->value) {
        free(macro); /* Free the macro structure if strdup fails */
        printf("[TYPES:] Macro value memory allocation failed\n");
        return 1;
    }

    return _insert_item(macro_table, macro);
}

int insert_label(const char *name, const int value) {
    /* Allocate memory for new label structure */
    Label *label = (Label *)_safe_malloc(sizeof(Label), "label_table");
    if (!label)
        return 1;

    label->name = name;
    label->value = value;

    return _insert_item(label_table, label);
}

int insert_symbol(const char *name, const int value, const int type) {
    /* Allocate memory for new symbol structure */
    Symbol *symbol = (Symbol *)_safe_malloc(sizeof(Symbol), "symbol_table");
    if (!symbol)
        return 1;

    symbol->name = name;
    symbol->symbol_value = value;
    symbol->symbol_type = type;

    return _insert_item(symbol_table, symbol);
}

void *get_item(const Table *instance, const char *name) {
    char *item_name;
    int i;
    if (!instance || !instance->content || !name) {
        return NULL;
    }

    for (i = 0; i < instance->count; i++) {
        item_name = ((Symbol *)instance->content[i])->name;

        if (item_name && strcmp(item_name, name) == 0) {
            return instance->content[i];
        }
    }

    return NULL; /* Not found */
}
