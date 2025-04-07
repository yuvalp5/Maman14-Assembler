; Assembly file that will pass first pass but fail during second pass

; Define a macro for later use
mcro print_hello
    prn "Hello"
mcroend

; This file has syntactically valid code that will pass the first pass
; But will fail in the second pass due to symbol resolution issues

START: mov r1, r2
       add #5, r3
       
; Error 1: Using an undefined label in an instruction (detected in second pass)
       jmp &NONEXISTENT_LABEL   ; This label doesn't exist

; Error 2: Entry declaration for an undefined symbol
.entry UNDEFINED_ENTRY          ; This symbol is never defined

; Error 3: Label defined both as extern and entry (conflict)
.extern CONFLICT_SYMBOL
.entry CONFLICT_SYMBOL          ; Error: Symbol cannot be both extern and entry

; Error 4: Using an external symbol as data
.extern EXTERNAL_DATA
       mov EXTERNAL_DATA[r1], r2  ; Valid in first pass, but will cause issues in second pass

; Error 5: Reference to undefined macro
undefined_macro                 ; This macro was never defined

; Error 6: Using a label before it's defined (forward reference)
; This would pass first pass but might cause issues in second pass
       lea LATER_DEFINED, r6    ; Forward reference
       
; Some valid code and definitions
VALID_LABEL: mov r3, r4
             print_hello        ; Using the defined macro
             
LATER_DEFINED: .data 10, 20, 30  ; The symbol referenced earlier

END: stop 