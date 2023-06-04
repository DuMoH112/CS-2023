%include'./lib/library.asm'

section .data
    arr dd  'u', 'd', 'd', 'u', 'u', 'd', 'd', 'u'
    n   dd  8

section .text
global _start
_start:
    xor eax, eax    ; eax = 0
    mov ecx, [n]    ; counter = n
    mov ebx, arr    ; ebx = адрес 0-го байта 0-го эл-та
    jmp start_iter

start_iter:
    mov edx, [ebx]
    cmp edx, 'u'
    je up
    jmp down

next_iter:
    add ebx, 4      ; ebx = адрес 0-го байта след. эл-та
    dec ecx         ; counter--
    jnz start_iter
    jmp if_end

up:
    inc eax
    jmp next_iter

down:
    dec eax
    jmp next_iter

if_end:
    cmp eax, 0
    jne zero
    mov eax, 1
    jmp stop_iter

zero:
    mov eax, 0
    jmp stop_iter

stop_iter:
    call print_number
    call exit