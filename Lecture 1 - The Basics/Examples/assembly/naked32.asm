; barebones asm file (32-bit)
; will assemble into the relevant machine code and nothing else

BITS 32

    mov     edx, eax
    mov     bl, [eax]
    test    bl, bl
    jz      loop_end
loop_start:
    inc     eax
    mov     bl, [eax]
    test    bl, bl
    jnz     loop_start
loop_end:
    sub     eax, edx
