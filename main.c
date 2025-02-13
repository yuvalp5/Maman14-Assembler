#include <stdio.h>
/**
 * @brief main. recive input here, so on...
 */

 /*
TODO:
    [x] well, rtfm
    [x] handle user file input as args


Things to keep in mind:

    an initial TODO list. later, tasks will be splittel into corrosponding files.

    registers: 24 bits each, r0 to r7

    PSW (program status word): keeps track of flags

    a command is 1 to 3 words

    words are encided as hex values

    commands: mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, jsr, red, prn, rts, stop

    sentences are separated by newlines and tabs (\n, \t)

    sentence types are:

        empty
        comment- starts with ;
        instruction
        command

        more info in PDF

    table of all operands and theoir respective binary codes - values set at runtime

    separate data from instructions

    all errors are to be encountered. an error wont stop the program


    fields:

        label-
            starts the symbol and ends with :
            can only be declared once
            case sensitive
            cannot be a keyword

        number-
            starts with optional + or -
            base-10
            only Z


        string-
            ascii series
            printable
            surroded by "
            space is also a part of the string


Run order:

    receive file of source to run

    pre-assembler- read input, then convert to actual instructions
    if failed raise error; thenn halt
    else save output
    algorythm in p.n. 40 (16)

    declare symbols, then declared symbols are given values dynamically

    go over raw source twice: forst find labels and replace them with values (real locations), second to write actual machine code



    convery symbols to machine code

    make a file of ready-to-run example



Things to do:

    1. implement described above commands:
        2 operands
        1 operand
        0 operands

    2. implement macros (dynamic allocation)
        no מקוננות macros
        macro name cant be an instruction name
        no need to check macroend phrase



 */

int main() {
    printf("%s", "Hello World!");
}
