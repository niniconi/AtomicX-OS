#include "link.h"

.section .text

ENTRY(_start)
    movq $0x10,%rax
    mov %rax,%ds
    mov %rax,%es
    mov %rax,%gs
    mov %rax,%fs
    mov %rax,%ss
    movq $0x7c00,%rsp

    lgdt GDTPtr(%rip)
    lidt IDTPtr(%rip)

    movq $0x10,%rax
    mov %rax,%ds
    mov %rax,%es
    mov %rax,%gs
    mov %rax,%fs
    mov %rax,%ss
    movq $0x7c00,%rsp

    //load PML4
    movq $0x101000,%rax
    movq %rax,%cr3

    jmp kernel

GLOBAL(PML4)
    .org 0x1000
    __PML4T:
        .quad 0x102003
        .fill 255,8,0
        .quad 0x102003
        .fill 255,8,0
    .org 0x2000
    __PDPT:
        .quad 0x00103003
        .quad 0x00000083//1GB PAGE
        .quad 0x40000083//1GB PAGE
        .fill 512-3,8,0
    .org 0x3000
    __PDT:
        // the default page size
        .quad 0x0000083//1MB PAGE
        .quad 0x0200083//1MB PAGE
        .quad 0x0400083//1MB PAGE
        .quad 0x0600083//1MB PAGE
        .quad 0x0800083//1MB PAGE
        .quad 0x0a00083//1MB PAGE
        .quad 0x0c00083//1MB PAGE
        .quad 0x0e00083//1MB PAGE
        .quad 0x1000083//1MB PAGE
        .quad 0x1200083//1MB PAGE
        .quad 0x1400083//1MB PAGE
        .quad 0x1600083//1MB PAGE
        .quad 0x1800083//1MB PAGE
        .quad 0x1a00083//1MB PAGE
        .quad 0x1c00083//1MB PAGE
        .quad 0x1e00083//1MB PAGE
        .quad 0x2000083//1MB PAGE

        .quad 0xe0000083//0x2200000
        .quad 0xe0200083
        .quad 0xe0400083
        .quad 0xe0600083
        .quad 0xe0800083
        .quad 0xe0a00083
        .quad 0xe1000083
        .quad 0xe1200083

        .quad 0x0104003
        .fill 512-26,8,0
    .org 0x4000
    __PT:
        .quad 0x0000000
        .fill 512-1,8,0

.section .data

GLOBAL(GDT)
    .quad 0x0000000000000000//null descriptor
    .quad 0x00209e0000000000//kernel code 64 bit segment descriptor
    .quad 0x0000920000000000//kernel data 64 bit segment descriptor
    .quad 0x0020fe0000000000//user code 64 bit segment descriptor
    .quad 0x0000f20000000000//user data 64 bit segment descriptor
    .quad 0x00cf9e000000ffff//kernel code 32 bit segment descriptor
    .quad 0x00cf92000000ffff//kernel data 32 bit segment descriptor
    .quad 0x00cffe000000ffff//user code 32 bit segment descriptor
    .quad 0x00cff2000000ffff//user data 32 bit segment descriptor
    .fill 10,8,0//tss descriptor
GDTEnd:

GLOBAL(IDT)
    .fill 512,8,0
IDTEnd:

GLOBAL(TSS)
    .fill 26,4,0
TSSEnd:

IDTPtr:
    .word IDTEnd-IDT-1
    .quad IDT
GDTPtr:
    .word GDTEnd-GDT-1
    .quad GDT
