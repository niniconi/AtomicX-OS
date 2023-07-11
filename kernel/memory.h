#ifndef __MEMORY_H
#define __MEMORY_H

#define PhyToVirtAddr(PhyAddr) (void *)(0xffff800000000000UL + (PhyAddr))
#define VirtToPhyAddr(VirtAddr) (void *)((VirtAddr) - 0xffff800000000000UL)

int init_memory_management_unit();

struct zone{
    unsigned long phy_addr;
    unsigned long length;
};

struct page{
    unsigned long virt_addr;
    unsigned long phy_addr;
    struct zone zone;
};

struct e820_struct{
    unsigned long offset;
    unsigned long length;
    unsigned int type;
};

struct memory_management{
    unsigned long cr3;

    unsigned long _start;
    unsigned long _end;

    unsigned long page_count;
    unsigned long phy_length;

    unsigned int e820_length;
    struct e820_struct memory_struct[32];
};

extern unsigned long _start;
extern unsigned long _text;
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _rodata;
extern unsigned long _erodata;
extern unsigned long _bss;
extern unsigned long _ebss;
extern unsigned long _end;
#endif
