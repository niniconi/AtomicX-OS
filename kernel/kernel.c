#include "keyboard.h"
#include "print.h"
#include "lib.h"
#include "trap.h"
#include "interrupt.h"
#include "disk.h"

extern array_queue keyboard_buf;

void kernel(){
    init_ppos();
    debug("build time(%s)\n",__TIME__);

    char cpuid_buf[16] = {0};
    unsigned int *a,*b,*c,*d;
    a=(unsigned int *)&cpuid_buf[12];
    b=(unsigned int *)&cpuid_buf[8];
    c=(unsigned int *)&cpuid_buf[4];
    d=(unsigned int *)&cpuid_buf[0];

    cpuid(0x00, 0x00, a, b, c, d);
    info("CPUID max base main-leaf id %#010x\n", *a);
    info("CPU label %s\n", cpuid_buf);
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
    color_print(COL_WHITE, COL_BLACK, "%s", cpuid_buf);
    cpuid(0x80000008, 0x00, a, b, c, d);
    color_print(COL_WHITE, COL_BLACK, "%s\n", cpuid_buf);

    info("init trap\n");
    init_trap();

    info("init interrupt\n");
    init_interrupt();


    register_intr_handle(0x21, keyboard_handle,init_keybord, NULL, 0);

    while (1)analysis_keycode();
}
