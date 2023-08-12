#ifndef __GATE_H
#define __GATE_H
#include "lib.h"
#include "print.h"

struct idt_desc{
    unsigned long low;
    unsigned long high;
};
extern struct idt_desc IDT;

#define GATE_CALL_TYPE 0x0e
#define GATE_INT_TYPE 0x0e
#define GATE_TRAP_TYPE 0x0f

extern void set_gate(void * desc,void (*entry)(),unsigned long type,unsigned long dpl);
__asm__(
        "set_gate:\n\t"
        //get phy offset of default_interupt
        "shlq $16,%rsi\n\t"
        "shrq $16,%rsi\n\t"
        "movq %rdi,%rbx\n\t"
        //set offset [15:00]
        "shlq $48,%rsi\n\t"
        "shrq $48,%rsi\n\t"
        //set offset [31:16]
        "shrq $16,%rbx\n\t"
        "shlq $48,%rbx\n\t"
        "orq %rbx,%rsi\n\t"

        //set segment selector [31:16]
        "movq $0x08,%rbx\n\t"
        "shlq $16,%rbx\n\t"
        "orq %rbx,%rsi\n\t"

        "shlq $8,%rdx\n\t"
        /* "orq $0x01,%rdx\n\t"//set ist = 1 */
        "shlq $32,%rdx\n\t"
        "orq %rdx,%rsi\n\t"

        "movq $0x08,%rbx\n\t"
        "orq %rcx,%rbx\n\t"
        "shlq $44,%rbx\n\t"
        "orq %rbx,%rsi\n\t"

        "movq %rsi,(%rdi)\n\t"
        "addq $0x08,%rdi\n\t"
        "movq $0x00,(%rdi)\n\t"

        "ret \n\t"
        );

extern void inline set_intr_gate(unsigned long id,void (*entry)()){
#ifdef GATE_INFO
    info("register interrupt addr:%#018x, entry:%#018x\n",&IDT+id,entry);
#endif
    set_gate(&IDT+id, entry, GATE_INT_TYPE,0);
}
extern void inline set_trap0_gate(unsigned long id,void (*entry)()){
#ifdef GATE_INFO
    info("register (dpl=0) trap addr:%#018x, entry:%#018x\n",&IDT+id,entry);
#endif
    set_gate(&IDT+id, entry, GATE_TRAP_TYPE,0);
}
extern void inline set_trap3_gate(unsigned long id,void (*entry)()){
#ifdef GATE_INFO
    info("register (dpl=3) trap addr:%#018x, entry:%#018x\n",&IDT+id,entry);
#endif
    set_gate(&IDT+id, entry, GATE_TRAP_TYPE,3);
}
#endif
