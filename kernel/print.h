#ifndef __PRINT_H
#define __PRINT_H

#define COL_BLACK 0x00000000
#define COL_BLUE  0x000000ff
#define COL_GREEN 0x0000ff00
#define COL_RED   0x00ff0000

struct position{
    int x,y;
    int width,height;
    int charxs,charys;
    int *vromaddr;
};
extern struct position pos;

void printchar(int x,int y,char c,int bkcolor,int bgcolor);
#endif
