#ifndef __INTERRUPT_H
#define __INTERRUPT_H

struct intr_handle{
    unsigned long nr;
    void (*handle)(unsigned long nr);
    void (*install)();
    void (*uninstall)();
    unsigned long flags;
};

#define SAVEL_ALL \
    "pushq %r15 \n\t" \
    "pushq %r14 \n\t" \
    "pushq %r13 \n\t" \
    "pushq %r12 \n\t" \
    "pushq %r11 \n\t" \
    "pushq %r10 \n\t" \
    "pushq %r9 \n\t" \
    "pushq %r8 \n\t" \
    "pushq %rdi \n\t" \
    "pushq %rsi \n\t" \
    "pushq %rbp \n\t" \
    "pushq %rdx \n\t" \
    "pushq %rcx \n\t" \
    "pushq %rbx \n\t" \
    "pushq %rax \n\t"

#define RESTORE_ALL \
    "popq %rax \n\t" \
    "popq %rbx \n\t" \
    "popq %rcx \n\t" \
    "popq %rdx \n\t" \
    "popq %rbp \n\t" \
    "popq %rsi \n\t" \
    "popq %rdi \n\t" \
    "popq %r8 \n\t" \
    "popq %r9 \n\t" \
    "popq %r10 \n\t" \
    "popq %r11 \n\t" \
    "popq %r12 \n\t" \
    "popq %r13 \n\t" \
    "popq %r14 \n\t" \
    "popq %r15 \n\t" \
    "iretq \n\t"

#define init_interrupt() \
    init_8259A();

#define STR_LABEL(LABEL) #LABEL
#define IRQ_NAME(IRQ) IRQ_handle##IRQ
#define IRQ(IRQ) \
    extern void IRQ_NAME(IRQ)(void); \
    __asm__( \
            STR_LABEL(IRQ_handle##IRQ)":\n\t" \
            SAVEL_ALL \
            "movq $" STR_LABEL(IRQ) ",%rdi \n\t"\
            "call do_IRQ \n\t" \
            RESTORE_ALL )

void init_8259A();
void register_intr_handle(unsigned long nr,void (*handle)(unsigned long nr),void (*install)(),void (*uninstall)(),unsigned long flags);
void unregister_intr_handle(unsigned long nr);

#endif
