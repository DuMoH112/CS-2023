%include'./lib/library.asm'

section .data
    arr dd  3, 2, 1, 5, 4
    n   dd  5
    res dd -1, -1, -1, -1, -1

section .text
global _start
_start:
    xor eax, eax        ; eax - последний тёплый день
    mov ecx, 1          ; counter = 1
    mov ebx, arr+4      ; ebx = адрес 0-го байта 1-го эл-та
    jmp start_iter

start_iter:
    mov edx, [ebx]
    cmp edx, [ebx-4]
    jl set_new_res
    cmp edx, eax
    jg next_iter
    cmp eax, 0
    jne set_res

next_iter:
    add ebx, 4          ; ebx = адрес 0-го байта след. эл-та
    inc ecx             ; counter--
    cmp ecx, [n]
    je print_res
    jmp start_iter

set_new_res:
    mov eax, [ebx-4]
    jmp set_res

set_res:
    mov [res+ecx*4], eax
    jmp next_iter

print_res:
    mov ecx, [n]        ; counter = n
    mov ebx, res        ; ebx = адрес 0-го байта 0-го эл-та
    jmp print_iter

print_iter:
    mov eax, [ebx]
    call print_number
    add ebx, 4          ; ebx = адрес 0-го байта след. эл-та
    loop print_iter
    call exit