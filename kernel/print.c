#include "font.h"
#include "print.h"

struct position pos;

void printchar(int x,int y,char c,int bkcolor,int bgcolor){
    int *vstart = pos.vromaddr + y*pos.charys*pos.width+x*pos.charxs;
    int i,j;
    char tmp;
    unsigned char *font = font_ascii[c];

    for (i=0;i<16;i++) {
        tmp = *(font+i);
        for (j=0;j<8;j++) {
            if (tmp & 0x01<<j) *(vstart+j) =bkcolor;
            else *(vstart+j) = bgcolor;
        }
        vstart += pos.width;
    }
}
