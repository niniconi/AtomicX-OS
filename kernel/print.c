#include "font.h"
#include "print.h"
#include "lib.h"
#include <stdarg.h>

struct position pos;

char buf[4096];

extern int inline todigital(const char **s){
    int ret = 0;
    while(**s >='0' && **s <= '9')ret=ret*10+*((*s)++) - '0';
    return ret;
}
int number(char * buf,unsigned long number,unsigned long flags,unsigned int R,unsigned int bit_length){
    int i,length=bit_length,num_length;
    unsigned long tmp;
    char *num_sys = NULL;

    if(flags & NUM_CAPITAL)num_sys = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    else num_sys = "0123456789abcdefghijklmnopqrstuvwxyz";
    if(R<2 || R > 36)return 0;

    for(num_length=0,tmp=number;tmp!=0;num_length++)
        tmp /= R;
    if (length == 0) length = num_length;

    if(flags & NUM_XSIGN && flags & NUM_LEFT){
        *buf++='0';
        *buf++='x';
    }

    for(i=0;i<length-num_length;i++){
        if(flags & NUM_SPACE)*buf++=' ';
        else if (flags & NUM_ZERO)*buf++='0';
    }

    if(flags & NUM_XSIGN && flags & NUM_RIGHT){
        *buf++='0';
        *buf++='x';
    }

    for(i=num_length - 1;i>=0;i--,number/=R)
        *(buf+i)=num_sys[number % R];

    if(flags & NUM_XSIGN)length+=2;
    return length;
}

int vsprintf(char *buf,const char * format,va_list ap){
    unsigned long flags;
    int bit_length = 0;
    while(*format != '\0'){
        flags = 0;
        if(*format == '%'){
            format++;
            switch (*format) {
                case '-':
                case '+':
                case ' ':
                case '#':
                    flags |= NUM_XSIGN;
                    if(*(++format) == '0'){
                        format++;
                        bit_length = todigital(&format);
                        flags |= NUM_ZERO | NUM_LEFT;
                    }else{
                        bit_length = todigital(&format);
                        flags |= NUM_SPACE | NUM_RIGHT;
                    }
                    break;
                case '0':
                    break;
            }
            switch(*format){
                case 'd':
                    buf += number(buf, va_arg(ap,int), 0, 10,bit_length);
                    break;
                case 'l':
                    format++;
                    switch (*format) {
                        case 'd':
                            buf += number(buf, va_arg(ap, long), 0, 10, bit_length);
                            break;
                    }
                    break;
                case 'x':
                    buf += number(buf, va_arg(ap, long), flags, 16,bit_length);
                    break;
                case 'X':
                    buf += number(buf, va_arg(ap, long), flags | NUM_CAPITAL, 16,bit_length);
                    break;
                case 'c':
                    *buf++=va_arg(ap, long);
                    break;
                case 's':
                    for (char *str = va_arg(ap,char *); *str != '\0'; str++)
                        *buf++=*str;
                    break;
                case '%':
                    *buf++='%';
                    break;
            }
            format++;
            continue;
        }
        *(buf++) = *format;
        format++;
    }
    *(buf++) = '\0';
    return 0;
}

void roll(unsigned long flags,unsigned int lines){
    void *source,*destination;
    unsigned int size = 0;
    if(flags & ROLL_DOWN){
        destination = pos.vromaddr;
        source = pos.vromaddr + lines*pos.charys*pos.width;
        size = (pos.height/pos.charys - lines)*pos.charys*pos.width*sizeof(int);
    }
    memcpy(source, destination, size);
}

void color_print(unsigned int bkcolor,unsigned int bgcolor,const char *format,...){
    va_list ap;
    char *buf = (char *)buf;
    int i;

    va_start(ap, format);
    vsprintf(buf,format,ap);
    va_end(ap);

    buf = (char *)buf;
    for(;*buf != '\0';buf++){
        switch (*buf) {
            case '\n':
                pos.y++;
                if(pos.y >= pos.height/pos.charys){
                    roll(ROLL_DOWN, 1);
                    pos.y--;
                }
                pos.x=0;
                continue;
            case '\b':
                pos.x--;
                if(pos.x < 0)pos.x=0;
                printchar(pos.x, pos.y, ' ', bkcolor, bgcolor);
                continue;
            case '\t':
                for (i=0; i<8; i++){
                    printchar(pos.x++, pos.y, ' ', bkcolor, bgcolor);
                    if(pos.x >= pos.width/pos.charxs){
                        pos.y++;
                        if(pos.y >= pos.height/pos.charys){
                            roll(ROLL_DOWN, 1);
                            pos.y--;
                        }
                        pos.x=0;
                        break;
                    }
                }
                continue;
        }
        printchar(pos.x, pos.y, *buf, bkcolor, bgcolor);
        pos.x++;
        if(pos.x >= pos.width/pos.charxs){
            pos.y++;
            if(pos.y >= pos.height/pos.charys){
                roll(ROLL_DOWN, 1);
                pos.y--;
            }
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
