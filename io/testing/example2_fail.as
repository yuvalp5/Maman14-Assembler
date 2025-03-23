; AUTO GENERATED

; Example Assembly Program with errors for testing
; The assembler should detect and report these errors

; Error 1: Invalid macro definition (missing mcroend)
mcro bad_macro
    mov r1, r2
    add #5, r3
; mcroend intentionally missing

; Error 2: Duplicated label definition
DUPLICATE: .data 10
DUPLICATE: .data 20   ; Error: Label already defined

; Error 3: Invalid register name
mov rx, r1            ; Error: rx is not a valid register (should be r0-r7)

; Error 4: Invalid number of operands
add r1                ; Error: add requires two operands

; Error 5: Invalid addressing mode
lea #5, r3            ; Error: lea does not support immediate addressing for source

; Error 6: Undefined label
mov UNDEFINED_LABEL, r2  ; Error: UNDEFINED_LABEL not defined

; Error 7: Using a reserved word as a label
mov: .data 100        ; Error: mov is a reserved word (instruction name)

; Error 8: Invalid directive name
.invalid 100          ; Error: .invalid is not a recognized directive

; Error 9: Incorrect operand types
cmp r1, STR           ; Error: Cannot compare register with string address

; Error 10: Syntax error in data directive
.data 5, , 10         ; Error: Empty value between commas

; Error 11: Invalid characters in label
123LABEL: .data 5     ; Error: Label must start with a letter

; Error 12: Label too long (31 characters max)
ABCDEFGHIJKLMNOPQRSTUVWXYZ12345: .data 5  ; Error: Label exceeds max length

; Valid sections to make sure the file has some correct parts
VALID: mov r1, r2
.string "This is valid"
.data 1, 2, 3

END: stop 