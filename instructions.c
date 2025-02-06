/**
 *@brief Instructions implementation
 */

#include "instructions.h"

void add(int a, register rg) {
    a = a + rg;
}

void sub(int a, register rg) {
    a = a - rg;
}

void lea(int a, register rg) {
    a = &rg;
}

void jmp(int a) {
    // TODO: make a current line to proccess
}

void bne(int a) {
    // TODO
}

void jsr(int a) {
    // TODO
}

void red(int a) {
    fscanf(stdin, "%d", &a);
}

void prn(int a) {
    fprintf(stdout, "%d", a);
}

void stop() {
    exit(0);
}
