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

#endif
