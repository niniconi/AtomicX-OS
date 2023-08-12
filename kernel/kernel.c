#include <atomicx/fs/fat32.h>
#include <atomicx/keyboard.h>
#include <atomicx/print.h>
#include <atomicx/trap.h>
#include <atomicx/interrupt.h>
#include <atomicx/disk.h>
#include <atomicx/memory.h>
#include <atomicx/date.h>
#include <atomicx/disk.h>
#include <atomicx/vfs.h>
#include <atomicx/debug.h>
#include <lib.h>

extern array_queue keyboard_buf;

void kernel(){
    init_ppos();
    debug("build time(%s)\n",__TIME__);

    char cpuid_buf[16] = {0};
    unsigned int *a,*b,*c,*d;

    a=(unsigned int *)&cpuid_buf[0];
    b=(unsigned int *)&cpuid_buf[4];
    d=(unsigned int *)&cpuid_buf[8];
    c=(unsigned int *)&cpuid_buf[12];

    cpuid(0x00, 0x00, a, b, c, d);
    info("CPUID max base main-leaf id %#010x\n", *a);
    info("CPU label %s\n", &cpuid_buf[4]);

    a=(unsigned int *)&cpuid_buf[0];
    b=(unsigned int *)&cpuid_buf[4];
    c=(unsigned int *)&cpuid_buf[8];
    d=(unsigned int *)&cpuid_buf[12];
    cpuid(0x01, 0x00, a, b, c, d);
    info("CPU version infomation %#010x\n", *a);
    cpuid(0x80000000, 0x00, a, b, c, d);
    info("CPUID max sub-leaf id %#010x\n", *a);

    info("");
    cpuid(0x80000002, 0x00, a, b, c, d);
    color_print(COL_WHITE, COL_BLACK, "%s", cpuid_buf);
    cpuid(0x80000003, 0x00, a, b, c, d);
    color_print(COL_WHITE, COL_BLACK, "%s", cpuid_buf);
    cpuid(0x80000004, 0x00, a, b, c, d);
    color_print(COL_WHITE, COL_BLACK, "%s\n", cpuid_buf);


    struct cmos_date date;
    memset(&date,0,sizeof(struct cmos_date));
    get_cmos_date(&date);
    info("DATE:year %d month %d day %d %d:%d:%d\n",date.year,date.month,date.day,date.hour,date.minute,date.second);

    init_trap();

    init_interrupt();

    init_memory_management_unit();

    register_intr_handle(0x21, keyboard_handle,init_keybord, NULL, 0);
    register_intr_handle(0x2f, disk_handle,init_disk,NULL,0);

    init_vfs();

    struct page * page1 = NULL;
    struct page * page2 = NULL;
    struct page * page3 = NULL;
    struct page * page4 = NULL;

    //TODO BUG 1
    /* page1 = alloc_pages(10,PAGEA_DIRECT,0); */
    /* page2 = alloc_pages(20,PAGEA_DIRECT,0); */
    /* free_pages(20,page2); */
    /* free_pages(10,page1); */
    /* page1 = alloc_pages(3,PAGEA_DIRECT,0); */
    /* free_pages(3,page1); */
    /* page1 = alloc_pages(10,0,0); */
    /* page2 = alloc_pages(10,0,0); */
    /*                 _debug_print_page(); */
    /* free_pages(10,page2); */
    /* free_pages(10,page1); */
    while (1)kb_analysis_keycode();
}
