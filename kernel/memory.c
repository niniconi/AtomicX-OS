#include "memory.h"
#include "print.h"
#include "lib.h"

struct memory_management memory_management;

int init_memory_management_unit(){
    unsigned long cr3;

    get_CR3(cr3);
    info("CR3 = %#018x\n",cr3);

    /*
     * get memory infomation from 0x7e00
     * init memory_management
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

    unsigned long total_useful_memory = 0;
    for(i=0;i<32;i++)
        if(memory_management.memory_struct[i].type == 0x01)total_useful_memory+=memory_management.memory_struct[i].length;

    info("useful memory total:%#018x\n",total_useful_memory);

    memory_management.phy_length = total_useful_memory;
    memory_management._start = _start;
    memory_management._end = _end;


    info("init memory management unit\n");
}
