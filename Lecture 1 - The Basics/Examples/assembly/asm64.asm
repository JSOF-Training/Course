global _asm_function

; this is a comment

section .data ; initialized data, dx directives

section .bss  ; uninitialized data, resx directives

section .text ; rather confusingly, assembly code goes here

_asm_function:
    xor rax, rax
    ret
