#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef _DEBUG
#include "print.h"
#include "lib.h"

__asm__(
        "get_rip:\n\t"
        "movq 0x00(%rsp),%rax \n\t"
        "ret \n\t"
        );

#define _debug_print_rip() \
    do { \
        register unsigned long rip = 0; \
        __asm__ __volatile__("call get_rip\n\t":"=a"(rip)::); \
        info("RIP = %#018x\n",rip); \
    }while(0)

#define _debug_wait(c) \
    for(int __i = 0;__i<c;__i++) { nop(); }

#else

#define _debug_print_rip()
#define _debug_wait(c)

#endif
#endif