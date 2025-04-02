# C Assembler

<b>This is the final assignment of course 20465 of The Open University - MAMAN 14.</b>

<i>Built by Oren B. and Yuval P.</i>

## Description

This program is an implementation of a (very) basic assembler

## Details

### Implemented Functionalities:

-   error handling
-   instructions - `mov`, `cmp`, `add`, `sub`, `lea`, `clr`, `not`, `inc`, `dec`, `jmp`, `bne`, `jsr`, `red`, `prn`, `rts`, `stop`
-   registers 0-7
-   todo - add more details as progress is made

### Structure:

`/`: main, Project files such as directions, this README, and git files

`/core`: Logic and backend innerworkings

`inter`: Proccessing of input before execution

# TODOs:

<i>more detailed TODOs are in src</i>

-   manage `#include`s
- check: different types for data and instructions? keep binary codes in tables?
- make use of RESERVED_KW
- DOC
- implement Memory type? end of p. 48 - machine code, type, text
- error checking and reporting- dont stop before ending reading stream
- define SUCCESS_CODE globally
- print methods in utils for debug
