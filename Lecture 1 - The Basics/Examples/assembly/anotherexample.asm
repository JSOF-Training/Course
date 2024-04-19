;https://courses.ics.hawaii.edu/ReviewICS312/morea/FirstProgram/ics312_nasm_first_program.pdf

segment .data
	integer1 dd 15 ; first int 
	integer2 dd 6 ; second int
segment .bss
	result resd 1 ; result
segment .text
global asm_main
asm_main:
	enter 0,0
	pusha
	mov eax, [integer1] ; eax = int1
	add eax, [integer2] ; eax = int1 + int2
	mov [result], eax ; result = int1 + int2
	popa
	mov eax, 0
	leave
	ret