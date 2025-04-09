; Test file that passes first pass but fails in second pass

; Define a macro that will be used correctly
mcro inc_and_print
    add #1, r1
    mov r1, r2
mcroend

; Define another macro for testing
mcro print_value
    mov r2, r3
    mov r3, r4
mcroend

; Start of code section
START: mov r1, r2        ; Initialize registers
       add #5, r3        ; Some valid operation

; Error 1: Trying to use a data label in an instruction
DATA_VAL: .data 100, 200, 300
         jmp DATA_VAL    ; Error: Can't jump to data label

; Error 2: Using an entry symbol that is defined as external
.extern CONFLICT_SYM
.entry CONFLICT_SYM      ; Error: Symbol can't be both extern and entry

; Some valid data definitions
STR1: .string "Test"
STR2: .string "Hello"

; Error 3: Using wrong addressing mode with external
.extern EXTERNAL_LABEL
       mov EXTERNAL_LABEL, r1  ; Valid in first pass, error in second

; Some more valid code
LOOP: mov r1, r2
      add r2, r3
      
; Error 4: Using string label as number
       lea STR1, r5     ; Valid in first pass, error in second

; Error 5: Using data label in branch
       jmp DATA_VAL     ; Valid in first pass, error in second

END: stop 