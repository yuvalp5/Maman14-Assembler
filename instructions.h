#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

// yuval - 2
void mov(int a, int b); // copy a to b
int cmp(int a, int b);  // compare a to b
void add(int a, int b); // add a to b
void sub(int a, int b); // substract a from b
void lea(int a, int b); // load effective address...?

// yuval -4
void clr(int a); // reset a
void not(int a); // invert as
void inc(int a); // increment a
void dec(int a); // decrement a
void jmp(int a); // jump to a
void bne(int a); // branch if not equal to zero
void jsr(int a); // jump to subroutine
void red(int a); // stdin to a
void prn(int a); // print a to stdout

// yuval -1
void rts();  // pop stack to PC
void stop(); // stop
