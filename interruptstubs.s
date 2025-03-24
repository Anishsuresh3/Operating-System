.section .text

.extern _ZN15InterruptManger13handleIntruptEhj

.int_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN15InterruptManger13handleIntruptEhj
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popla

    iret

.data:
    interruptnumber: .byte 0