#include "print.h"

void kernel(){
    int * vrom = (int *)0xffff800002200000;
    int i,j;
    for (i=0,j=0;i<1440;i++,j=0){
        while(j++<16)
            *(vrom + j*1440 + i)=0x000000ff;
    }
    for (i=0,j=16;i<1440;i++,j=16){
        while(j++<32)
            *(vrom + j*1440 + i)=0x0000ff00;
    }
    for (i=0,j=32;i<1440;i++,j=32){
        while(j++<48)
            *(vrom + j*1440 + i)=0x00ff0000;
    }
    for (i=0,j=48;i<1440;i++,j=48){
        while(j++<64)
            *(vrom + j*1440 + i)=0x00ffffff;
    }
    pos.x=0;
    pos.y=0;
    pos.width=1440;
    pos.height=900;
    pos.charxs=8;
    pos.charys=16;
    pos.vromaddr=(int *)0xffff800002200000;

    INFO("hello word!\n");
    ERROR("hello word!\n");
    WARRING("hello word!\n");
    while(1);
}
