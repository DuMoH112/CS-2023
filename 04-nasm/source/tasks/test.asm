    %include'./lib/library.asm'

    section .text
    global  _start

_start:
    mov     eax,            100
    add     eax,            156
    call    print_number
    call    exit