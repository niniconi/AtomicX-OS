#ifndef __MEMORY_H
#define __MEMORY_H

#define PhyToVirtAddr(PhyAddr) (void *)(0xffff800000000000UL + (PhyAddr))
#define VirtToPhyAddr(VirtAddr) (void *)((VirtAddr) - 0xffff800000000000UL)

#define PAGE4K_SIZE (1 << 12)
#define PAGE1M_SIZE (1 << 20)
#define PAGE1G_SIZE (1 << 30)

#define PAGE4K_MASK ~((2*PAGE4K_SIZE) - 1)
#define PAGE1M_MASK ~((2*PAGE1M_SIZE) - 1)
#define PAGE1G_MASK ~((2*PAGE1G_SIZE) - 1)

#define PAGE4K_ALIGN(addr) ((addr) & PAGE4K_MASK + ((addr) % PAGE4K_SIZE ? PAGE4K_SIZE:0))
#define PAGE1M_ALIGN(addr) ((addr) & PAGE1M_MASK + ((addr) % PAGE1M_SIZE ? PAGE1M_SIZE:0))
#define PAGE1G_ALIGN(addr) ((addr) & PAGE1G_MASK + ((addr) % PAGE1G_SIZE ? PAGE1G_SIZE:0))

/*
 * if set this bit
 *     phy_addr & (~0xffff800000000000UL) == virt_addr
 * else
 *     phy_addr & (~0xffff800000000000UL) != virt_addr
 */
#define PAGEA_DIRECT 0x01UL
#define PAGEA_SHARE 0x02UL

#define PAGE_KERNEL 0x01UL
#define PAGE_RD 0x02UL
#define PAGE_WR 0x04UL
#define PAGE_EXEC 0x08UL
#define PAGE_FREED 0x10UL

#ifndef DEFAULT_PAGE_SIZE
#define DEFAULT_PAGE_SIZE PAGE4K_SIZE
#endif

/*
 * reserve for memory management unit
 */
#define KERNEL_REVD_PAGE_CNT 20

int init_memory_management_unit();
struct page * alloc_pages(unsigned int cnt, unsigned long option, unsigned long flags);
int free_pages(int cnt, struct page * dpage);

struct zone{
    unsigned long phy_addr;
    unsigned long length;

    unsigned long used;
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
