#ifndef __MEMORY_H
#define __MEMORY_H

#include "slab.h"

#define PhyToVirtAddr(PhyAddr) (void *)(0xffff800000000000UL + (unsigned long)(PhyAddr))
#define VirtToPhyAddr(VirtAddr) (void *)((unsigned long)(VirtAddr) - 0xffff800000000000UL)

#define PAGE4K_SIZE (1 << 12)
#define PAGE2M_SIZE (1 << 21)
#define PAGE1G_SIZE (1 << 30)

#define PAGE4K_MASK (~(PAGE4K_SIZE - 1))
#define PAGE2M_MASK (~(PAGE2M_SIZE - 1))
#define PAGE1G_MASK (~(PAGE1G_SIZE - 1))

#define PAGE4K_ALIGN(addr) ((addr) & PAGE4K_MASK + ((addr) % PAGE4K_SIZE ? PAGE4K_SIZE:0))
#define PAGE2M_ALIGN(addr) ((addr) & PAGE2M_MASK + ((addr) % PAGE2M_SIZE ? PAGE2M_SIZE:0))
#define PAGE1G_ALIGN(addr) ((addr) & PAGE1G_MASK + ((addr) % PAGE1G_SIZE ? PAGE1G_SIZE:0))

#define ADDR_PML4_MASK 0x0000ff8000000000UL
#define ADDR_PDPT_MASK 0x0000007fc0000000UL
#define ADDR_PDT_MASK  0x000000003fe00000UL
#define ADDR_PT_MASK   0x00000000001ff000UL

/*
 * if set this bit
 *     virt_addr & (~0xffff800000000000UL) == phy_addr
 * else
 *     virt_addr & (~0xffff800000000000UL) != phy_addr
 */
#define PAGEA_DIRECT 0x01UL
#define PAGEA_SHARE 0x02UL

#define PAGE_KERNEL 0x01UL
#define PAGE_RD 0x02UL
#define PAGE_WR 0x04UL
#define PAGE_EXEC 0x08UL
#define PAGE_FREED 0x10UL

#define ZONE_ALLOW_USE 0x01L

#ifndef DEFAULT_PAGE_SIZE
#define DEFAULT_PAGE_SIZE PAGE4K_SIZE
#endif

/*
 * reserve for memory management unit
 */
#define KERNEL_REVD_PAGE_CNT 20

/*
 * segment selector
 */
#define KERNEL64_CS 0x08
#define KERNEL64_DS 0x10
#define USER64_CS 0x18
#define USER64_DS 0x20
#define KERNEL32_CS 0x28
#define KERNEL32_DS 0x30
#define USER32_CS 0x38
#define USER32_DS 0x40

int init_memory_management_unit();
struct page * alloc_pages(unsigned int cnt, unsigned long option, unsigned long flags);
int free_pages(int cnt, struct page * dpage);
void *init_page_table(void * page_table,struct page * pages,unsigned int count);

struct zone{
    unsigned long phy_addr;
    unsigned long length;

    unsigned long flags;

    unsigned long used;
    struct zone *next;
};

struct page{
    unsigned long virt_addr;
    unsigned long phy_addr;
    struct zone *zone;

    unsigned long flags;

    unsigned int pid;

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
    unsigned long ktext;
    unsigned long ketext;
    unsigned long kdata;
    unsigned long kedata;
    unsigned long krodata;
    unsigned long kerodata;
    unsigned long kbss;
    unsigned long kebss;
    unsigned long end;

    unsigned long page_count;
    unsigned long used_phy;
    unsigned long phy_size;

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
