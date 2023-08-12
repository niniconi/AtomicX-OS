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

    movq switch_seg(%rip),%rax
    pushq $0x08
    pushq %rax
    lretq

switch_seg:
    .quad entry_64
entry_64:
    movq $0x10,%rax
    mov %rax,%ds
    mov %rax,%es
    mov %rax,%gs
    mov %rax,%fs
    mov %rax,%ss
    movq $0xffff800000007c00,%rsp

    // set the default idt
    call set_default_idt

    // set default tss descriptor and tss sttuct
    call set_default_tss
    // movw $0x08*0x09,%ax
    // ltr %ax

    jmp kernel

set_default_idt:
    leaq IDT(%rip),%rdx
    leaq default_interupt(%rip),%rax

    //get phy offset of default_interupt
    shlq $16,%rax
    shrq $16,%rax
    movq %rax,%rbx
    //set offset [15:00]
    shlq $48,%rax
    shrq $48,%rax
    //set offset [31:16]
    shrq $16,%rbx
    shlq $48,%rbx
    orq %rbx,%rax

    //set segment selector [31:16]
    movq $0x08,%rbx
    shlq $16,%rbx
    orq %rbx,%rax

    movq $0xe00,%rbx
    shlq $32,%rbx
    orq %rbx,%rax

    movq $0x08,%rbx
    shlq $44,%rbx
    orq %rbx,%rax

    movq $256,%rcx
    _set_idt:
        movq %rax,(%rdx)
        addq $0x08,%rdx
        movq $0x00,(%rdx)
        addq $0x08,%rdx
    loop _set_idt
    ret

set_default_tss:
    movq $0xffff800000007c00,%rax
    leaq TSS(%rip),%rdi

    movq %rax,4(%rdi)
    movq %rax,12(%rdi)
    movq %rax,20(%rdi)
    movq %rax,36(%rdi)
    movq %rax,44(%rdi)
    movq %rax,52(%rdi)
    movq %rax,60(%rdi)
    movq %rax,68(%rdi)
    movq %rax,76(%rdi)
    movq %rax,84(%rdi)
    movw $0x00,102(%rdi)

    movq %rdi,%rax
    movq $5,%rcx
    leaq GDT(%rip),%rdi
    addq $9*8,%rdi

    _set_tss:
        // set segment length [15:00]
        movw $0xffff,0(%rdi)

        //set segment base [23:00]
        movw %ax,16(%rdi)
        shrq $16,%rax
        movb %al,32(%rdi)
        shrq $8,%rax

        movw $0x0f89,40(%rdi)
        movb %al,56(%rdi)
        shrq $8,%rax
        movl %eax,64(%rdi)

        addq $16,%rdi
    loop _set_tss
    ret

ENTRY(default_interupt)
    movq $0x00ff0000,%rdi
    movq $0x00000000,%rsi
    leaq default_interupt_msg(%rip),%rdx
    movq $0,%rax
    callq color_print
    loop:
        jmp loop

default_interupt_msg:
    .asciz "unexpected interrupt number...\n\0"

.org 0x1000
__PML4T:
    .quad 0x102003
    .fill 255,8,0
    .quad 0x102003
    .fill 255,8,0
.org 0x2000
__PDPT:
    .quad 0x00103003
    .quad 0x40000083//1GB PAGE
    .quad 0x80000083//1GB PAGE
    .quad 0xc0000083//1GB PAGE
    .quad 0x100000083//1GB PAGE
    .quad 0x140000083//1GB PAGE
    .quad 0x180000083//1GB PAGE
    .quad 0x1c0000083//1GB PAGE
    .quad 0x200000083//1GB PAGE
    .quad 0x2e0000083//1GB PAGE
    .fill 512-10,8,0
.org 0x3000
__PDT:
    // the default page size
    .quad 0x0000083//2MB PAGE
    .quad 0x0200083//2MB PAGE
    .quad 0x0400083//2MB PAGE
    .quad 0x0600083//2MB PAGE
    .quad 0x0800083//2MB PAGE
    .quad 0x0a00083//2MB PAGE
    .quad 0x0c00083//2MB PAGE
    .quad 0x0e00083//2MB PAGE
    .quad 0x1000083//2MB PAGE
    .quad 0x1200083//2MB PAGE
    .quad 0x1400083//2MB PAGE
    .quad 0x1600083//2MB PAGE
    .quad 0x1800083//2MB PAGE
    .quad 0x1a00083//2MB PAGE
    .quad 0x1c00083//2MB PAGE
    .quad 0x1e00083//2MB PAGE
    .quad 0x2000083//2MB PAGE

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
