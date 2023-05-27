#include "link.h"

ENTRY(div_error)
    movq (%rsp),%rdi// get error code
    leaq do_div_error(%rip),%rax
    jmp SAVEL_ALL
ENTRY(debug_exception)
    leaq do_debug_exception(%rip),%rax
    jmp SAVEL_ALL
ENTRY(nmi)
    leaq do_nmi(%rip),%rax
    jmp SAVEL_ALL
ENTRY(breakpoint)
    leaq do_breakpoint(%rip),%rax
    jmp SAVEL_ALL
ENTRY(overflow)
    leaq do_overflow(%rip),%rax
    jmp SAVEL_ALL
ENTRY(out_bounds)
    leaq do_out_bounds(%rip),%rax
    jmp SAVEL_ALL
ENTRY(undefine)
    leaq do_undefine(%rip),%rax
    jmp SAVEL_ALL
ENTRY(device_exception)
    leaq do_device_exception(%rip),%rax
    jmp SAVEL_ALL
ENTRY(double_error)
    leaq do_double_error(%rip),%rax
    jmp SAVEL_ALL
ENTRY(coprocessor_error)
    leaq do_coprocessor_error(%rip),%rax
    jmp SAVEL_ALL
ENTRY(invaild_tss)
    leaq do_invaild_tss(%rip),%rax
    jmp SAVEL_ALL
ENTRY(undefine_segment)
    leaq do_undefine_segment(%rip),%rax
    jmp SAVEL_ALL
ENTRY(SS_segment_error)
    leaq do_SS_segment_error(%rip),%rax
    jmp SAVEL_ALL
ENTRY(general_protect)
    leaq do_general_protect(%rip),%rax
    jmp SAVEL_ALL
ENTRY(invaild_page)
    leaq do_invaild_page(%rip),%rax
    jmp SAVEL_ALL
ENTRY(x87_FPU)
    leaq do_x87_FPU(%rip),%rax
    jmp SAVEL_ALL
ENTRY(alignment_check)
    leaq do_alignment_check(%rip),%rax
    jmp SAVEL_ALL
ENTRY(mechine_check)
    leaq do_mechine_check(%rip),%rax
    jmp SAVEL_ALL
ENTRY(SIMD_float_exception)
    leaq do_SIMD_float_exception(%rip),%rax
    jmp SAVEL_ALL
ENTRY(VM_exception)
    leaq do_VM_exception(%rip),%rax
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
    movq %rsp,%rsi//get regs
    call *%rax
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
