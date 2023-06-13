%include'./lib/library.asm'

section .data
    arr dd 0,0,1,0,1,1,1,0
    n dd 8
    max dd 0

section .text
global  _start
_start:
    xor eax, eax    ; eax = 0
    mov ecx, [n]    ; counter = n
    mov ebx, arr    ; ebx = адрес 0-го байта 0-го эл-та
    jmp start_iter

start_iter:
    mov edx, [ebx]
    cmp edx, 0
    je if_zero
    jmp if_one

next_iter:
    add ebx, 4      ; ebx = адрес 0-го байта след. эл-та
    dec ecx         ; counter--
    jnz start_iter
    jmp if_end

if_zero:
    cmp eax, [max]
    jg set_max
    xor eax, eax    ; eax = 0
    jmp next_iter

if_one:
    inc eax
    jmp next_iter

set_max:
    mov [max], eax
    xor eax, eax    ; eax = 0
    jmp next_iter

if_end:
    cmp eax, [max]
    jle stop_iter
    mov [max], eax
    jmp stop_iter    

stop_iter:
    mov eax, [max]
    call    print_number
    call    exit