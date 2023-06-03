#include "link.h"

ENTRY(div_error)
    movq (%rsp),%rsi// get error code
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_div_error(%rip),%rax
    call *%rax
    jmp RESTORE_ALL
ENTRY(debug_exception)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_debug_exception(%rip),%rax
    call *%rax
    jmp RESTORE_ALL
ENTRY(nmi)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_nmi(%rip),%rax
    call *%rax
    jmp RESTORE_ALL
ENTRY(breakpoint)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_breakpoint(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(overflow)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_overflow(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(out_bounds)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_out_bounds(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(undefine)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_undefine(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(device_exception)
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
ENTRY(invaild_page)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_invaild_page(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(x87_FPU)
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
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_mechine_check(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(SIMD_float_exception)
    call SAVEL_ALL
    movq %rsp,%rdi//get regs
    leaq do_SIMD_float_exception(%rip),%rax
    call *%rax
    jmp SAVEL_ALL
ENTRY(VM_exception)
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
    ret
RESTORE_ALL:
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
