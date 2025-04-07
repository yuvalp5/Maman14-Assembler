; Assembly file that will fail during the first pass

; Error 1: Symbol with reserved word name
mov: .data 100      ; Error: Using 'mov' (reserved instruction) as a symbol

; Error 2: Invalid addressing mode for instruction
START: lea #5, r3   ; Error: lea doesn't support immediate addressing mode for source

; Error 3: Invalid operand count
      add r5        ; Error: add requires two operands

; Error 4: Symbol redefinition
LABEL: .data 10, 20, 30
LABEL: .string "Duplicate label" ; Error: Symbol already defined

; Error 5: Invalid register number
      mov r9, r2    ; Error: Invalid register r9 (valid are r0-r7)

; Error 6: Invalid immediate value
      mov #abc, r1  ; Error: Invalid immediate value (should be a number)

; Error 7: Invalid index addressing
      inc X[r8]     ; Error: Invalid register in index addressing

; Error 8: Memory overflow (max is 4096)
.data 1, 2, 3, 4, 5, 6 ; Normal data
; [Imagine 4090+ more numbers would be here to cause overflow]

; Error 9: Invalid syntax in .data directive
.data 5,,10        ; Error: Empty value between commas

; Error 10: Wrong operand type for instruction
      stop #5       ; Error: stop doesn't take any operands

; Some valid code for context
VALID: mov r1, r2
       inc r3
       .string "This is valid"
       
END: stop 