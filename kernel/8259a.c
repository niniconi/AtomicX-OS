#include <atomicx/8259a.h>
#include <atomicx/print.h>
#include <lib.h>

void init_8259a(){
    //set master 8259A
    io_out8(0x20, 0x11);
    io_out8(0x21, 0x20);
    io_out8(0x21, 0x04);
    io_out8(0x21, 0x01);

    //set slave 8259A
    io_out8(0xa0, 0x11);
    io_out8(0xa1, 0x28);
    io_out8(0xa1, 0x02);
    io_out8(0xa1, 0x01);

    //set slave/master 8259A
    io_out8(0x21, 0xfd);
    io_out8(0xa1, 0xff);

    sti();

    info("init interrupt 8259a\n");
}
