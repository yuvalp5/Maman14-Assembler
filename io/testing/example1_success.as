; AUTO GENERATED

; Example Assembly Program based on the project specification

; Define macros
mcro print_a
    mov r3, r5
    prn #'a'
mcroend

mcro inc_r7
    inc r7
    mov r1, r7
mcroend

; External and entry declarations
.extern EXTERNAL_FUNC
.entry MAIN_ENTRY
.entry DATA_START

; Main program
MAIN: add #5, r1      ; Add immediate value 5 to r1
MAIN_ENTRY: mov r1, r2    ; Move content of r1 to r2

; Using a macro
print_a               ; This will be expanded by the pre-assembler

; Loop demonstration
LOOP: inc r3          ; Increment r3
      cmp r3, #10     ; Compare r3 with immediate value 10
      bne &LOOP       ; Branch if not equal back to LOOP

; Demonstrating different addressing modes
      lea STR, r6     ; Load effective address of STR to r6
      inc_r7          ; Use another macro
      jsr EXTERNAL_FUNC ; Jump to subroutine (external)
      mov #-1, r4     ; Move immediate negative value to r4
      
; Using stop to end program
END: stop

; Data definitions
STR: .string "This is a test string"
DATA_START: .data 100, -25, 12, 0
NUMS: .data 9, -100, 45 