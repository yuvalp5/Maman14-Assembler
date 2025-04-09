; Assembly file that passes first pass but fails during second pass only

; Define some macros that will be properly expanded
mcro increment
    inc r1
mcroend

mcro decrement
    dec r2
mcroend

; Some valid code that will pass syntax checking in first pass
START: mov r1, r3
       add #5, r4
       
; Use macros correctly (will be expanded by pre-processor)
       increment
       decrement
       
; Valid instructions with forward references
; These will pass first pass but cause issues in second pass
       jmp &FUTURE_LABEL     ; This label is defined later, forward reference is valid in first pass
       
; Valid entry declaration that will be checked in second pass
.entry NONEXISTENT_SYMBOL    ; This symbol is never defined, but first pass just collects entry declarations
       
; Define an external symbol
.extern EXTERNAL_SYMBOL

; Create a conflict between entry and extern in different parts of the file
.entry EXTERNAL_SYMBOL       ; This conflicts with the extern declaration, but is only checked in second pass

; Use external symbol with invalid addressing mode (only detected in second pass)
       mov EXTERNAL_SYMBOL, r5   ; Valid in first pass, checked in second pass

; Now define the label used in forward reference for validity in first pass
FUTURE_LABEL: stop

; Add some more valid code to ensure first pass runs normally
DATA_SEG: .data 100, 200, 300
STR_SEG: .string "This is a valid string"

END: stop 