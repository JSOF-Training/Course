; https://www.devdungeon.com/content/hello-world-nasm-assembler
; We'll learn about system calls later in the course
; For now you can accept that invoking "int 80h" with the correct register
; values will print a string to console
; This code is just given as an example of assembly syntax


; Define variables in the data section
section .data
	hello:     db 'Hello world!',10
	helloLen:  equ $-hello

; Code goes in the text section
section .text

GLOBAL _start
_start:
	mov eax,4            ; 'write' system call = 4
	mov ebx,1            ; file descriptor 1 = STDOUT
	mov ecx,hello        ; string to write
	mov edx,helloLen     ; length of string to write
	int 80h              ; call the kernel

	; Terminate program
	mov eax,1            ; 'exit' system call
	mov ebx,0            ; exit with error code 0
	int 80h              ; call the kernel
