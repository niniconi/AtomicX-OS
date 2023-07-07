#include "interrupt.h"
#include "gate.h"
#include "lib.h"
#include "print.h"

IRQ(0x20);
IRQ(0x21);
IRQ(0x22);
IRQ(0x23);
IRQ(0x24);
IRQ(0x25);
IRQ(0x26);
IRQ(0x27);
IRQ(0x28);
IRQ(0x29);
IRQ(0x2a);
IRQ(0x2b);
IRQ(0x2c);
IRQ(0x2d);
IRQ(0x2e);
IRQ(0x2f);
IRQ(0x30);
IRQ(0x31);
IRQ(0x32);
IRQ(0x33);
IRQ(0x34);
IRQ(0x35);
IRQ(0x36);
IRQ(0x37);

void (*IRQ_handles[0x18])(void)={
    IRQ_NAME(0x20),
    IRQ_NAME(0x21),
    IRQ_NAME(0x22),
    IRQ_NAME(0x23),
    IRQ_NAME(0x24),
    IRQ_NAME(0x25),
    IRQ_NAME(0x26),
    IRQ_NAME(0x27),
    IRQ_NAME(0x28),
    IRQ_NAME(0x29),
    IRQ_NAME(0x2a),
    IRQ_NAME(0x2b),
    IRQ_NAME(0x2c),
    IRQ_NAME(0x2d),
    IRQ_NAME(0x2e),
    IRQ_NAME(0x2f),
    IRQ_NAME(0x30),
    IRQ_NAME(0x31),
    IRQ_NAME(0x32),
    IRQ_NAME(0x33),
    IRQ_NAME(0x34),
    IRQ_NAME(0x35),
    IRQ_NAME(0x36),
    IRQ_NAME(0x37)
};

struct intr_handle handles[0x18];

void do_IRQ(unsigned long nr){
    interrupt("do IRQ%#04x\n",nr);
    handles[nr-0x20].handle(nr);
}

void register_intr_handle(unsigned long nr,void (*handle)(unsigned long nr),void (*install)(),void (*uninstall)(),unsigned long flags){
    struct intr_handle * hdle = &handles[nr-0x20];
    hdle->nr = nr;
    hdle->handle = handle;
    hdle->install = install;
    hdle->uninstall = uninstall;
    hdle->flags = flags;

    if(hdle->install != NULL)hdle->install();
}

void unregister_intr_handle(unsigned long nr){
    struct intr_handle * hdle = &handles[nr-0x20];
    hdle->nr = 0;
    hdle->handle = NULL;
    hdle->install = NULL;
    hdle->uninstall = NULL;
    hdle->flags = 0;

    if(hdle->uninstall != NULL)hdle->uninstall();
}

void init_8259A(){
    int i;
    for (i=0x20; i<=0x37; i++) {
        set_intr_gate(i, IRQ_handles[i-0x20]);
    }

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
    io_out8(0xa1, 0x3f);

    sti();
}
