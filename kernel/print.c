#include "font.h"
#include "print.h"
#include <stdarg.h>

struct position pos;

char buf[4096];

extern int inline isdigital(const char *s){
    int ret = 0;
    while(*s >='0' && *s <= '9')ret=ret*10+*(s++) - '0';
    return ret;
}
int number(char * buf,unsigned long number,unsigned long flags,unsigned int R){
    int i,length;
    unsigned long tmp;
    char *num_sys = "0123456789abcdefghijklmnopqrstuvwxyz";

    if(flags & NUM_CAPITAL)
        for(i=10;i<36;i++)*(num_sys+i) -= 32;
    if(flags & NUM_XSIGN){
        *buf++ = '0';
        *buf++ = 'x';
    }
    if(R<2 || R > 36)return 0;

    for(length=0,tmp=number;tmp!=0;length++)
        tmp /= R;

    for(int i=length-1;i>=0;i--,number/=R)
        *(buf+i)=num_sys[number % R];

    return length;
}

int vsprintf(char *buf,const char * format,va_list ap){
    unsigned long flags = 0;
    int R = 16;
    while(*format != '\0'){
        if(*format == '%'){
            format++;
            switch (*format) {
                case '-':
                case '+':
                case ' ':
                case '#':
                    flags |= NUM_XSIGN;
                    int num;
                    if(*(++format) == '0'){
                        num = isdigital(++format);
                        for (num++;num>=0;num--)
                            *(buf+num)='0';
                    }else{
                        num = isdigital(format);
                        for (num++;num>=0;num--)
                            *(buf+num)=' ';
                    }
                    format++;
                    break;
                case '0':
                    break;
            }
            long num = va_arg(ap,long);
            switch(*format){
                case 'd':
                    buf += number(buf, num, 0, 10);
                    format++;
                    break;
                case 'x':
                    buf += number(buf, num, 0, 16);
                    format++;
                    break;
                case 'X':
                    buf += number(buf, num, NUM_CAPITAL, 16);
                    format++;
                    break;
            }
        }
        *(buf++) = *format;
        format++;
    }
    *(buf++) = '\0';
    return 0;
}

void color_print(unsigned int bkcolor,unsigned int bgcolor,const char *format,...){
    va_list ap;
    char *buf = (char *)buf;
    va_start(ap, format);
    vsprintf(buf,format,ap);
    va_end(ap);

    buf = (char *)buf;
    for(;*buf != '\0';buf++){
        if(*buf=='\n'){
            pos.y++;
            pos.x=0;
            continue;
        }else if (*buf=='\b') {
            pos.x--;
            if(pos.x < 0)pos.x=0;
            printchar(pos.x, pos.y, ' ', bkcolor, bgcolor);
            continue;
        }
        printchar(pos.x, pos.y, *buf, bkcolor, bgcolor);
        pos.x++;
        if(pos.x >= pos.width/pos.charxs){
            pos.y++;
            pos.x=0;
        }
    }
}

void printchar(int x,int y,char c,unsigned int bkcolor,unsigned int bgcolor){
    int *vstart = pos.vromaddr + y*pos.charys*pos.width+x*pos.charxs;
    int i,j;
    char tmp;
    unsigned char *font = font_ascii[c];

    for (i=0;i<16;i++) {
        tmp = *(font+i);
        for (j=0;j<8;j++) {
            if (tmp & 0x80>>j) *(vstart+j) =bkcolor;
            else *(vstart+j) = bgcolor;
        }
        vstart += pos.width;
    }
}
