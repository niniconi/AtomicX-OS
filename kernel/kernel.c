#include "print.h"

void kernel(){
    int * vrom = (int *)0xffff8000000a0000;
    int i,j;
    for (i=0,j=0;i<1440;i++,j=0){
        while(j++<4)
            *(vrom + j*1440 + i)=0x000000ff;
    }
    for (i=0,j=4;i<1440;i++,j=4){
        while(j++<8)
            *(vrom + j*1440 + i)=0x0000ff00;
    }
    pos.x=0;
    pos.y=0;
    pos.width=1440;
    pos.height=900;
    pos.charxs=8;
    pos.charys=16;
    pos.vromaddr=(int *)0xffff8000000a0000;
    printchar(0, 0, 'H', COL_BLUE, COL_BLACK);
    printchar(1, 0, 'e', COL_GREEN, COL_BLACK);
    printchar(2, 0, 'l', COL_RED, COL_BLACK);
    printchar(3, 0, 'l', COL_GREEN, COL_BLACK);
    printchar(4, 0, 'o', COL_GREEN, COL_BLACK);
    printchar(5, 0, ',', COL_GREEN, COL_BLACK);
    printchar(6, 0, 'w', COL_GREEN, COL_BLACK);
    printchar(7, 0, 'o', COL_GREEN, COL_BLACK);
    printchar(8, 0, 'r', COL_GREEN, COL_BLACK);
    printchar(9, 0, 'l', COL_GREEN, COL_BLACK);
    printchar(10, 0, 'd', COL_GREEN, COL_BLACK);
    while(1);
}
