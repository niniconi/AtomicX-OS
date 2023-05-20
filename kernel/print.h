#ifndef __PRINT_H
#define __PRINT_H

#define COL_BLACK   0x00000000
#define COL_BLUE    0x000000ff
#define COL_GREEN   0x0000ff00
#define COL_RED     0x00ff0000
#define COL_YELLOW  0x00ffff00
#define COL_ORANGE  0x00ff9900
#define COL_WHITE   0x00ffffff

#define NUM_SIGN     0x0000000000000001UL
#define NUM_XSIGN    0x0000000000000002UL
#define NUM_CAPITAL  0x0000000000000004UL

#define ERROR(msg,args...) \
    do{ \
        color_print(COL_RED,COL_BLACK,"[error ]:");\
        color_print(COL_WHITE,COL_BLACK,msg,##args);\
    }while(0) \

#define INFO(msg,args...) \
    do{ \
        color_print(COL_GREEN,COL_BLACK,"[info  ]:");\
        color_print(COL_WHITE,COL_BLACK,msg,##args);\
    }while(0) \

#define WARRING(msg,args...) \
    do{ \
        color_print(COL_ORANGE,COL_BLACK,"[warring]:");\
        color_print(COL_WHITE,COL_BLACK,msg,##args);\
    }while(0) \

struct position{
    int x,y;
    int width,height;
    int charxs,charys;
    int *vromaddr;
};
extern struct position pos;

void printchar(int x,int y,char c,unsigned int bkcolor,unsigned int bgcolor);
void color_print(unsigned int bkcolor,unsigned int bgcolor,const char *format,...);

#endif
