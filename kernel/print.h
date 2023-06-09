#ifndef __PRINT_H
#define __PRINT_H
#include <stdarg.h>

#define COL_BLACK   0x00000000
#define COL_BLUE    0x000000ff
#define COL_GREEN   0x0000ff00
#define COL_RED     0x00ff0000
#define COL_YELLOW  0x00ffff00
#define COL_ORANGE  0x00ff9900
#define COL_WHITE   0x00ffffff

#define E_BKCOLFLAGS "0;38;2"
#define E_COL_BLACK  "0;0;0"
#define E_COL_BLUE   "0;0;255"
#define E_COL_GREEN  "0;255;0"
#define E_COL_RED    "255;0;0"
#define E_COL_YELLOW "255;255;0"
#define E_COL_ORANGE "255;153;0"
#define E_COL_WHITE  "255;255;255"

#define NUM_SIGN     0x01UL << 0
#define NUM_CAPITAL  0x01UL << 1
#define NUM_XSIGN    0x01UL << 3
#define NUM_SPACE    0x01UL << 4
#define NUM_ZERO     0x01UL << 5
#define NUM_LEFT     0x01UL << 6
#define NUM_RIGHT    0x01UL << 7

#define ROLL_UP      0x01UL << 0
#define ROLL_DOWN    0x01UL << 1

#ifdef LOG_ERROR
#define error(msg,args...) \
    do{ \
        color_print(COL_RED,COL_BLACK,"[error    ]:");\
        color_print(COL_WHITE,COL_BLACK,msg,##args);\
    }while(0)
#else
#define error(msg,args...)
#endif

#ifdef LOG_INFO
#define info(msg,args...) \
    do{ \
        color_print(COL_GREEN,COL_BLACK,"[info     ]:");\
        color_print(COL_WHITE,COL_BLACK,msg,##args);\
    }while(0)
#else
#define info(msg,args...)
#endif

#ifdef LOG_WARRING
#define warring(msg,args...) \
    do{ \
        color_print(COL_ORANGE,COL_BLACK,"[warring  ]:");\
        color_print(COL_WHITE,COL_BLACK,msg,##args);\
    }while(0)
#else
#define warring(msg,args...)
#endif

#ifdef LOG_INTERRUPT
#define interrupt(msg,args...) \
    do{ \
        color_print(COL_YELLOW,COL_BLACK,"[interrupt]:");\
        color_print(COL_WHITE,COL_BLACK,msg,##args);\
    }while(0)
#else
#define interrupt(msg,args...)
#endif

#ifdef LOG_DEBUG
#define debug(msg,args...) \
    do{ \
        color_print(COL_BLUE,COL_BLACK,"[debug    ]:");\
        color_print(COL_WHITE,COL_BLACK,msg,##args);\
    }while(0)
#else
#define debug(msg,args...)
#endif


struct position{
    int x,y;
    int width,height;
    int charxs,charys;
    int *vromaddr;
};
extern struct position pos;

void printchar(int x,int y,char c,unsigned int bkcolor,unsigned int bgcolor);
void color_print(unsigned int bkcolor,unsigned int bgcolor,const char *format,...);
#define print(format,args...) color_print(COL_WHITE,COL_BLACK,format,##args)
int vsprintf(char *buf,const char * format,va_list ap);
void init_ppos();

#endif
