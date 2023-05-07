#include "link.h"
ENTRY(_start)
    jmp kernel


ENTRY(PML4)
    .org 0x1000
    __PML4T:
        .quad 0x102003
    .org 0x2000
    __PDPT:
        .quad 0x103083
    .org 0x3000
    __PDT:
    .org 0x4000
    __PT:

ENTRY(GDT)
    .quad 0x0000000000000000//null descriptor
    .quad 0x00609e0000000000//kernel code 64 bit segment descriptor
    .quad 0x0000920000000000//kernel data 64 bit segment descriptor
    .quad 0x0060fe0000000000//user code 64 bit segment descriptor
    .quad 0x0000f20000000000//user data 64 bit segment descriptor
    .quad 0x00cf9e000000ffff//kernel code 32 bit segment descriptor
    .quad 0x00cf92000000ffff//kernel data 32 bit segment descriptor
    .quad 0x00cffe000000ffff//user code 32 bit segment descriptor
    .quad 0x00cff2000000ffff//user data 32 bit segment descriptor
    .fill 10,8,0//tss descriptor
GDTEnd:

ENTRY(IDT)
    .fill 512,8,0
IDTEnd:

ENTRY(TSS)
    .fill 26,4,0
TSSEnd:

IDTPtr:
    .word IDTEnd-IDT
    .quad IDT
GDTPtr:
    .word GDTEnd-GDT
    .quad GDT
