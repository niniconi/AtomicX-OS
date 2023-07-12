#include "memory.h"
#include "print.h"
#include "lib.h"
#include "slab.h"

struct memory_management memory_management;

struct page * alloc_pages(int cnt,unsigned flags){
    struct page * page = (struct page *)memory_management.end;
    page->flags = flags;
    memory_management.page_count += cnt;
    memory_management.used_phy += cnt * PAGE4K_SIZE;
    memory_management.end += cnt * PAGE4K_SIZE;
}

void free_page(struct page * page){
    struct page * root = memory_management.page;
}

int init_memory_management_unit(){
    unsigned long cr3 = 0;

    get_CR3(cr3);
    info("CR3 = %#018x\n",cr3);

    /*
     * get memory infomation from 0x7e00
     * to init memory_management
     */
    memset(&memory_management,0,sizeof(memory_management));

    int i;
    unsigned char * e820 = PhyToVirtAddr(0x7e00);

    for(i=0;i<32;i++,e820 += 20){
        if(*(unsigned int *)(e820 + 16) == 0x00)break;
        info("Offset:%#018x,Length:%#018x,Type:%#010x\n",*(unsigned long *)e820,*(unsigned long *)(e820 + 8),*(unsigned int *)(e820 + 16));

        memory_management.memory_struct[i].offset = *(unsigned long *)e820;
        memory_management.memory_struct[i].length = *(unsigned long *)(e820 + 8);
        memory_management.memory_struct[i].type = *(unsigned int *)(e820 + 16);
        memory_management.e820_length += 1;
    }

    memory_management.cr3 = cr3;
    memory_management.kstart = (unsigned long)&_start;
    memory_management.end = (unsigned long)&_end;
    memory_management.kend = (unsigned long)&_end;

    info("kernel kstart:%#018x,kend:%#018x\n",memory_management.kstart,memory_management.kend);

    /*
     * get total useful memory
     * init zone list
     */

    unsigned long total_useful_memory = 0;
    struct zone *zone = NULL;

    memory_management.zone = (struct zone *)memory_management.end;

    for(i=0;i < memory_management.e820_length;i++){
        if(memory_management.memory_struct[i].type == 0x01){
            zone = (struct zone *)memory_management.end;
            memset(zone,0,sizeof(struct zone));

            zone->phy_addr = memory_management.memory_struct[i].offset;
            zone->length = memory_management.memory_struct[i].length;
            memory_management.end += sizeof(struct zone);
            zone->next = (struct zone *)memory_management.end;

            total_useful_memory+=memory_management.memory_struct[i].length;
        }
    }

    info("useful memory total:%#018x\n",total_useful_memory);

    memory_management.phy_length = total_useful_memory;

    /*
     * init kernel used page
     */

    unsigned long kernel_size = memory_management.kend-memory_management.kstart;
    unsigned long kernel_page_cnt = kernel_size/PAGE4K_SIZE + (kernel_size % PAGE4K_SIZE?0:1);
    struct page * kpage = (struct page *)memory_management.end;
    struct page * last = NULL;
    for(i=0;i<kernel_page_cnt;i++){
        memset(kpage,0,sizeof(struct page));
        kpage->last = last;
        kpage->phy_addr = i*PAGE4K_SIZE + memory_management.kstart;
        kpage->flags = PAGE_EXEC | PAGE_RD | PAGE_WR | PAGE_KERNEL;
        kpage->virt_addr = (unsigned long)PhyToVirtAddr(kpage->phy_addr);
        kpage->next = kpage + 1;
        last = kpage;
        kpage++;
    }
    memory_management.page_count += kernel_page_cnt;
    memory_management.end += kernel_page_cnt * sizeof(struct page);
    memory_management.used_phy += kernel_page_cnt* sizeof(struct page);


    init_slab();

    info("init memory management unit\n");
}

