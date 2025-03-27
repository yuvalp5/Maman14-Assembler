#include "../../inter/pre_assembler.h"
#include <stdio.h>

int main() {
    char fname[] = "io/example1_success.as";
    char fname2[] = "io/example1_success.am";
    pre_assembler(fname, fname2);
    return 0;
}
