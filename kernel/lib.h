#ifndef __LIB_H
#define __LIB_H

#define NULL 0
#define nop() __asm__ __volatile__("nop \n\t":::)
#define cli() __asm__ __volatile__("cli \n\t":::)
#define sti() __asm__ __volatile__("sti \n\t":::)

extern void inline cpuid(unsigned long fn,unsigned long exfn,unsigned int *areg,unsigned int *breg,unsigned int *creg,unsigned int *dreg){
    __asm__ __volatile__(
            "cpuid \n\t"
            :"=a"(*areg),"=b"(*breg),"=c"(creg),"=d"(dreg)
            :"a"(fn),"c"(fn)
            :);
}

extern void inline memcpy(void * source,void *destination,unsigned long size){
    __asm__ __volatile__(
            "cld \n\t"
            "rep movsb\n\t"
            :
            :"D"(destination),"S"(source),"c"(size)
            :);
}

extern void inline io_out8(int port,char data){
    __asm__ __volatile__(
            "outb %%al,%%edx \n\t"
            :
            :"a"(data),"d"(port)
            :);
}
extern void inline io_out32(int port,int data){
    __asm__ __volatile__(
            "outl %%eax,%%edx\n\t"
            :
            :"a"(data),"d"(port)
            :);
}
extern char inline io_in8(int port){
    register char ret;
    __asm__ __volatile__(
            "inb %%edx,%0 \n\t"
            :"=r"(ret)
            :"d"(port)
            :);
    return ret;
}
extern int inline io_in32(int port){
    register int ret;
    __asm__ __volatile__(
            "inl %%edx,%0 \n\t"
            :"=r"(ret)
            :"d"(port)
            :);
}

typedef struct regs{
    unsigned long rax;
    unsigned long rbx;
    unsigned long rcx;
    unsigned long rdx;
    unsigned long rbp;
    unsigned long rsi;
    unsigned long rdi;
    unsigned long r8;
    unsigned long r9;
    unsigned long r10;
    unsigned long r11;
    unsigned long r12;
    unsigned long r13;
    unsigned long r14;
    unsigned long r15;
}regs;

#endif
