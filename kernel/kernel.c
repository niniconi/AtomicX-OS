#include "print.h"
#include "lib.h"

void kernel(){
    pos.x=0;
    pos.y=0;
    pos.width=1440;
    pos.height=900;
    pos.charxs=8;
    pos.charys=16;
    pos.vromaddr=(int *)0xffff800002200000;

    char cpuid_buf[16] = {0};
    unsigned int *a,*b,*c,*d;
    a=(unsigned int *)&cpuid_buf[12];
    b=(unsigned int *)&cpuid_buf[8];
    c=(unsigned int *)&cpuid_buf[4];
    d=(unsigned int *)&cpuid_buf[0];

    cpuid(0x00, 0x00, a, b, c, d);
    info("CPUID max base main-leaf id %#08x\n", *a);
    info("CPU label %s\n", cpuid_buf);
    cpuid(0x01, 0x00, a, b, c, d);
    info("CPU version infomation %#08x\n", *a);
    cpuid(0x80000000, 0x00, a, b, c, d);
    info("CPUID max sub-leaf id %#08x\n", *a);

    info("");
    cpuid(0x80000002, 0x00, a, b, c, d);
    color_print(COL_BLACK, COL_BLACK, "%s", cpuid_buf);
    cpuid(0x80000003, 0x00, a, b, c, d);
    color_print(COL_BLACK, COL_BLACK, "%s", cpuid_buf);
    cpuid(0x80000004, 0x00, a, b, c, d);
    color_print(COL_BLACK, COL_BLACK, "%s", cpuid_buf);
    cpuid(0x80000008, 0x00, a, b, c, d);
    color_print(COL_BLACK, COL_BLACK, "%s\n", cpuid_buf);

    while(1);
}
