#include "link.h"

/*
    The following is the stack memory distribution,After calling SAVEL_ALL
*/
SS = 0xb8
RSP = 0xb0
RFLAGS = 0xa8
CS = 0xa0
RIP = 0x98
ERRCODE = 0x90
FUNC = 0x88 //SAVEL_ALL return address
R15 = 0x80
R14 = 0x78
R13 = 0x70
R12 = 0x68
R11 = 0x60
R10 = 0x58
R9 = 0x50
R8 = 0x48
RDI = 0x40
RSI = 0x38
RBP = 0x30
RDX = 0x28
RCX = 0x20
RBX = 0x18
RAX = 0x10
ES = 0x08
DS = 0x00
REGS = 0x00

ENTRY(div_error)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_div_error(%rip),%rax
    call *%rax
    jmp RESTORE_ALL
ENTRY(debug_exception)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_debug_exception(%rip),%rax
    call *%rax
    jmp RESTORE_ALL
ENTRY(nmi)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_nmi(%rip),%rax
    call *%rax
    jmp RESTORE_ALL
ENTRY(breakpoint)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_breakpoint(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(overflow)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_overflow(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(out_bounds)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_out_bounds(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(undefine)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_undefine(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(device_exception)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_device_exception(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(double_error)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_double_error(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(coprocessor_error)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_coprocessor_error(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(invaild_tss)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_invaild_tss(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(undefine_segment)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_undefine_segment(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(SS_segment_error)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_SS_segment_error(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(general_protect)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_general_protect(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(page_fault)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_page_fault(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(x87_FPU)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_x87_FPU(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(alignment_check)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_alignment_check(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(mechine_check)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_mechine_check(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(SIMD_float_exception)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_SIMD_float_exception(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(VM_exception)
    pushq $0x00
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_VM_exception(%rip),%rax
    call *%rax
    jmp SAVEL_ALL

SAVEL_ALL:
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12
    pushq %r11
    pushq %r10
    pushq %r9
    pushq %r8
    pushq %rdi
    pushq %rsi
    pushq %rbp
    pushq %rdx
    pushq %rcx
    pushq %rbx
    pushq %rax
    movq %es,%rax
    pushq %rax
    movq %ds,%rax
    pushq %rax
    jmpq *FUNC(%rsp)
RESTORE_ALL:
    popq %rax
    movq %rax,%ds
    popq %rax
    movq %rax,%es
    popq %rax
    popq %rbx
    popq %rcx
    popq %rdx
    popq %rbp
    popq %rsi
    popq %rdi
    popq %r8
    popq %r9
    popq %r10
    popq %r11
    popq %r12
    popq %r13
    popq %r14
    popq %r15
    iretq
