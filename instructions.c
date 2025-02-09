/**
 *@brief Instructions implementation
 */

#include "instructions.h"
// First group of commands
void mov(int a, int b){
    b=a;
}

int cmp(int a, int b){
    return (a==b);
}

void add(int a, register rg) {
    a = a + rg;
}

void sub(int a, register rg) {
    a = a - rg;
}

void lea(int a, register rg) {
    a = &rg;
}

// Second group of commands
void clr(int a){
    a = 0;
}

void not(int a){
    a = ~a;
}

void inc(int a){
    a+=1;
}

void dec(int a){
    a-=1;
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

// Third group of commands

void rts(){
    // TODO - implement a stack data structure
    if (stack->stack_top == NULL){
        printf("no value to pop from the stack");
    }
    else{
    *pc = stack->stack_top
    stack -> size --;
    printf("value popped: %d\n", stack->stack_top);
    }
}

void stop() {
    exit(0);
}
