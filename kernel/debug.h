#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef _DEBUG
#include "print.h"
#include "memory.h"
#include "lib.h"

extern struct memory_management memory_management;

struct page *__page = NULL;
struct zone *__zone = NULL;

__asm__(
        "get_rip:\n\t"
        "movq 0x00(%rsp),%rax \n\t"
        "ret \n\t"
        );

#define _debug_print_rip() \
    do { \
        register unsigned long rip = 0; \
        __asm__ __volatile__("call get_rip\n\t":"=a"(rip)::); \
        debug("RIP = %#018x\n",rip); \
    }while(0)

#define _debug_wait(c) \
    for(int __i = 0;__i<c;__i++) { nop(); }

#define _debug_print_memory_mangement() \
    debug("page--->%#018x,zone--->%#018x,page_count--->%#010x,used_phy--->%#018x,phy_size--->%#018x\n", \
            memory_management.page, \
            memory_management.zone, \
            memory_management.page_count, \
            memory_management.used_phy, \
            memory_management.phy_size)

#define _debug_print_page() \
    do { \
        __page = memory_management.page; \
        int index = 0; \
        while(__page != NULL && !(__page->flags & PAGE_FREED)){ \
            debug("(%#06x)page_addr--->%#018x,virt_addr--->%#018x,phy_addr--->%#018x,zone--->%#018x,flags--->%#018x\n", \
                    index, \
                    __page, \
                    __page->virt_addr, \
                    __page->phy_addr, \
                    __page->zone, \
                    __page->flags); \
            __page = __page->next; \
            index++; \
        } \
        __page = NULL; \
    }while(0)

#define _debug_print_zone() \
    do { \
        __zone = memory_management.zone; \
        int index = 0; \
        while(__zone != NULL){ \
            debug("(%#06x)zone_addr--->%#018x,phy_addr--->%#018x,length--->%#018x,used--->%#018x\n", \
                    index, \
                    __zone, \
                    __zone->phy_addr, \
                    __zone->length, \
                    __zone->used); \
            __zone = __zone->next; \
            index++; \
        } \
        __zone = NULL; \
    }while(0)

#else

#define _debug_print_rip()
#define _debug_wait(c)
#define _debug_print_memory_mangement()
#define _debug_print_page()
#define _debug_print_zone()

#endif
#endif
