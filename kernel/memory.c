#include "memory.h"
#include "print.h"
#include "lib.h"
#include "slab.h"

struct memory_management memory_management;

struct page * alloc_pages(unsigned int cnt, unsigned long option, unsigned long flags){
    struct page * page = (struct page *)memory_management.end;
    struct page * epage = memory_management.page;
    struct page * ret_page = NULL;
    unsigned long virt_addr_start = memory_management.kstart + memory_management.page_count*DEFAULT_PAGE_SIZE;
    int i,j;

    if(cnt + memory_management.page_count >= ((KERNEL_REVD_PAGE_CNT*DEFAULT_PAGE_SIZE)/sizeof(struct page))){
        warring("alloc more page, please free some.");
    }

    flags &= ~PAGE_FREED;

    if(!(option & PAGEA_DIRECT)) virt_addr_start = 0;

    while(!(epage->flags & PAGE_FREED) && epage->next != NULL)epage = epage->next;

    if((epage->flags & PAGE_FREED) && (option & PAGEA_DIRECT)){
        unsigned int continue_page_length = 1;
        unsigned int continue_page_max_length = 1;
        struct page * continue_page = epage;
        struct page * continue_start_page = epage;
        struct page * continue_next_start_page = epage;
        struct page * continue_end_page = epage;

        while(continue_page != NULL) {
            if(continue_page->next != NULL) {
                if(continue_page->phy_addr + DEFAULT_PAGE_SIZE == continue_page->next->phy_addr) {
                    continue_page_length++;
                }else{
                    if(continue_page_length > continue_page_max_length){
                        continue_page_max_length=continue_page_length;
                        continue_start_page = continue_next_start_page;
                        continue_end_page = continue_page;
                    }
                    continue_next_start_page = continue_page->next;

                    continue_page_length = 1;
                }
            }
            continue_page = continue_page->next;
        }

        if(continue_page_max_length < cnt) goto NOT_FOUND_CONTINUE_PAGE;

        continue_start_page->last->next = continue_end_page->next;
        if(continue_end_page->next != NULL)continue_end_page->next->last = continue_start_page->last;

        epage->last->next = continue_start_page;
        continue_start_page->last = epage->last;

        epage->last = continue_end_page;
        continue_end_page->next = epage;

        continue_page = continue_start_page;
        for(i=0;i<cnt;i++){
            continue_page->flags = flags;
            continue_page->virt_addr = (unsigned long)PhyToVirtAddr(continue_page->phy_addr);

            continue_page = continue_page->next;
        }

        memory_management.page_count += cnt;
        memory_management.used_phy += cnt * DEFAULT_PAGE_SIZE;
        return continue_start_page;
    }

NOT_FOUND_CONTINUE_PAGE:

    if(epage->flags & PAGE_FREED)ret_page = epage;
    else ret_page = page;

    if ((epage->flags & PAGE_FREED) && !(option & PAGEA_DIRECT)) {
        for(i=0;i<cnt;i++){
            /*
             * use freed page's reserved phy address
             */
            epage->flags = flags;
            epage->virt_addr = virt_addr_start;
            virt_addr_start += DEFAULT_PAGE_SIZE;

            if(epage->next == NULL) break;
            epage = epage->next;
        }
        memory_management.end += (cnt - (i + 1)) * sizeof(struct page);

        i++;
        j=0;
        for(;i<cnt;i++){
            memset(page,0,sizeof(struct page));
            epage->next = page;
            page->last = epage;

            page->flags = flags;
            page->virt_addr = virt_addr_start;
            virt_addr_start += DEFAULT_PAGE_SIZE;
            page->phy_addr = (unsigned long)VirtToPhyAddr((j + memory_management.page_count)*DEFAULT_PAGE_SIZE + memory_management.kstart);
            j++;

            epage = page;
            page++;
        }
    }else if((epage->flags & PAGE_FREED) && (option & PAGEA_DIRECT)){
        struct page * sfpage = NULL;
        struct page * efpage = NULL;

        sfpage = epage;
        list_to_end(epage);
        efpage = epage;

        memory_management.end += cnt * sizeof(struct page);
        for (i=0;i<cnt;i++) {
            memset(page,0,sizeof(struct page));
            epage->next = page;
            page->last = epage;

            page->flags = flags;
            page->virt_addr = virt_addr_start;
            virt_addr_start += DEFAULT_PAGE_SIZE;
            page->phy_addr = (unsigned long)VirtToPhyAddr(page->virt_addr);

            epage = page;
            page++;
        }
        sfpage->last->next = efpage->next;
        efpage->next->last = sfpage->last;

        epage->next = sfpage;
        sfpage->last = epage;

        efpage->next = NULL;
    }else{
        memory_management.end += cnt * sizeof(struct page);
        for (i=0;i<cnt;i++) {
            memset(page,0,sizeof(struct page));
            epage->next = page;
            page->last = epage;

            /*
             * if has PAGEA_DIRECT,then phy_addr and virt_addr calculate the same result.
             * if not has PAGEA_DIRECT,then calculations are not the same.
             */
            page->flags = flags;
            page->virt_addr = virt_addr_start;
            virt_addr_start += DEFAULT_PAGE_SIZE;
            page->phy_addr = (unsigned long)VirtToPhyAddr((i + memory_management.page_count)*DEFAULT_PAGE_SIZE + memory_management.kstart);

            epage = page;
            page++;
        }
    }

    memory_management.page_count += cnt;
    memory_management.used_phy += cnt * DEFAULT_PAGE_SIZE;
    return ret_page;
}

int free_pages(int cnt, struct page * dpage){
    struct page * fpage = memory_management.page;
    struct page * epage = memory_management.page;
    struct page * fepage = NULL;
    int i;

    while(fpage != NULL){
        if(fpage == dpage){
            if(fpage->flags & PAGE_FREED)return 0;
            for(i=0;i<cnt;i++) {

                //keep phy addr
                dpage->flags = PAGE_FREED;
                dpage->virt_addr = NULL;
                dpage->zone->used -= DEFAULT_PAGE_SIZE;
                dpage->zone = NULL;

                if(i == cnt - 1)fepage = dpage;
                dpage = dpage->next;
            }

            fpage->last->next = dpage;
            if(dpage != NULL)dpage->last = fpage->last;

            list_to_end(epage);
            fpage->last = epage;
            epage->next = fpage;
            fepage->next = NULL;

            memory_management.page_count -= cnt;
            memory_management.used_phy -= cnt * DEFAULT_PAGE_SIZE;
            return 0;
        }
        fpage = fpage->next;
    }

    return 1;
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
    zone->next=NULL;

    info("useful memory total:%#018x\n",total_useful_memory);

    memory_management.phy_size = total_useful_memory;

    /*
     * init kernel used page
     */

    unsigned long kernel_size = memory_management.kend-memory_management.kstart;
    unsigned long kernel_page_cnt = kernel_size/DEFAULT_PAGE_SIZE + (kernel_size % DEFAULT_PAGE_SIZE ?1:0);
    struct page * kpage = (struct page *)memory_management.end;
    struct page * last = NULL;
    zone = NULL;
    memory_management.page = kpage;

    for(i=0;i<kernel_page_cnt;i++){
        memset(kpage,0,sizeof(struct page));
        kpage->last = last;
        kpage->phy_addr = i*DEFAULT_PAGE_SIZE + (unsigned long)VirtToPhyAddr(memory_management.kstart);

        zone = memory_management.zone;
        while(zone != NULL){
            if(kpage->phy_addr < zone->phy_addr+zone->length && kpage->phy_addr >= zone->phy_addr)break;
            zone = zone->next;
        }
        kpage->zone = zone;
        zone->used += DEFAULT_PAGE_SIZE;

        kpage->flags = PAGE_EXEC | PAGE_RD | PAGE_WR | PAGE_KERNEL;
        kpage->virt_addr = (unsigned long)PhyToVirtAddr(kpage->phy_addr);
        kpage->next = kpage + 1;
        last = kpage;
        kpage++;
    }
    memory_management.page_count += kernel_page_cnt;
    memory_management.end += kernel_page_cnt * sizeof(struct page);
    memory_management.used_phy += kernel_page_cnt * DEFAULT_PAGE_SIZE;

    /*
     * init kernel reserved page
     */
    unsigned long kernel_revd_page_cnt = KERNEL_REVD_PAGE_CNT;
    struct page * krpage = (struct page *)memory_management.end;
    last->next = krpage;

    for(i=0;i<kernel_revd_page_cnt;i++){
        memset(krpage,0,sizeof(struct page));
        krpage->last = last;
        krpage->phy_addr = last->phy_addr+DEFAULT_PAGE_SIZE;

        zone = memory_management.zone;
        while(zone != NULL){
            if(krpage->phy_addr < zone->phy_addr+zone->length && krpage->phy_addr >= zone->phy_addr)break;
            zone = zone->next;
        }
        krpage->zone = zone;
        zone->used += DEFAULT_PAGE_SIZE;

        krpage->flags = PAGE_RD | PAGE_WR | PAGE_KERNEL;
        krpage->virt_addr = (unsigned long)PhyToVirtAddr(krpage->phy_addr);
        krpage->next = krpage + 1;
        last = krpage;
        krpage++;
    }
    last->next = NULL;
    memory_management.page_count += kernel_revd_page_cnt;
    memory_management.end += kernel_revd_page_cnt * sizeof(struct page);
    memory_management.used_phy += kernel_revd_page_cnt * DEFAULT_PAGE_SIZE;


    init_slab();

    info("init memory management unit\n");
}

