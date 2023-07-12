#ifndef __MEMORY_H
#define __MEMORY_H

#define PhyToVirtAddr(PhyAddr) (void *)(0xffff800000000000UL + (PhyAddr))
#define VirtToPhyAddr(VirtAddr) (void *)((VirtAddr) - 0xffff800000000000UL)

#define PAGE4K_SIZE (2 << 12)
#define PAGE1M_SIZE (2 << 20)
#define PAGE1G_SIZE (2 << 30)

#define PAGE4K_MASK ~((2*PAGE4K_SIZE) - 1)
#define PAGE1M_MASK ~((2*PAGE1M_SIZE) - 1)
#define PAGE1G_MASK ~((2*PAGE1G_SIZE) - 1)

#define PAGE4K_ALIGN(addr) ((addr) & PAGE4K_MASK)
#define PAGE1M_ALIGN(addr) ((addr) & PAGE1M_MASK)
#define PAGE1G_ALIGN(addr) ((addr) & PAGE1G_MASK)

#define APAGE_DIRECT 0x01

#define PAGE_KERNEL 0x01
#define PAGE_RD 0x02
#define PAGE_WR 0x04
#define PAGE_EXEC 0x08

int init_memory_management_unit();

struct zone{
    unsigned long phy_addr;
    unsigned long length;
    struct zone *next;
};

struct page{
    unsigned long virt_addr;
    unsigned long phy_addr;
    struct zone *zone;

    unsigned long flags;

    struct page *last;
    struct page *next;
};

struct e820_struct{
    unsigned long offset;
    unsigned long length;
    unsigned int type;
};

struct memory_management{
    unsigned long cr3;

    unsigned long kstart;
    unsigned long kend;
    unsigned long end;

    unsigned long page_count;
    unsigned long used_phy;
    unsigned long phy_length;

    struct zone *zone;
    struct page *page;

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
