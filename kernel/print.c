#include "font.h"
#include "print.h"
#include "lib.h"
#include "memory.h"

#include <stdarg.h>

struct position pos;

char print_buf[4096];

extern int inline todigital(const char **s){
    int ret = 0;
    while(**s >='0' && **s <= '9')ret=ret*10+*((*s)++) - '0';
    return ret;
}

int number(char ** buf,unsigned long number,unsigned long flags,unsigned int R,int data_width,int data_precision){
    unsigned long tmp;
    const char * num_sys = "0123456789abcdefghijklmnopqrstuvwxyz";
    char sign;//if sign => number < 0
    int i;

    if(flags & NUM_CAPITAL) num_sys = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if(R<2 || R > 36) return 0;
    if(flags & NUM_SIGN){
        if(number & 0x8000000000000000){
            sign = 1;
            number = ~(number - 1);//abs(number)
        }else sign = 0;
    }
    if(data_precision == 0)
        for(tmp = number;tmp;data_precision++)tmp/=R;
    if(number == 0)data_precision = 1;
    if(data_width == 0 || data_width < data_precision){
        data_width = data_precision;
        if(flags & NUM_XSIGN)data_width += 2;
    }

    if(flags & NUM_LEFT){
        if(flags & NUM_SIGN){
            if(sign)
                *(*buf)++='-';
            else
                *(*buf)++='+';
            data_width--;
        }

        if(flags & NUM_XSIGN){
            *(*buf)++='0';
            *(*buf)++='x';
            data_width -= 2;
        }

        for (i=data_precision - 1;i>=0;number /= R,i--)
            *((*buf)+i) = num_sys[number % R];
        *buf += data_precision;
        data_width -= data_precision;
        for (i=0; i<data_width;i++) *(*buf)++ = ' ';
    }else{
        if(flags & NUM_ZERO){
            if(flags & NUM_SIGN){
                if(sign)
                    *(*buf)++='-';
                else
                    *(*buf)++='+';
                data_width--;
            }
            if (flags & NUM_XSIGN) {
                *(*buf)++='0';
                *(*buf)++='x';
                data_width -= 2;
            }
            for (i=0;i<data_width-data_precision;i++)
                *(*buf)++='0';
        }else{
            if(flags & NUM_XSIGN)
                data_width -= 2;
            for (i=0;i<data_width-data_precision;i++)
                *(*buf)++ = ' ';

            if(flags & NUM_SIGN){
                if(sign)
                    *(*buf)++='-';
                else
                    *(*buf)++='+';
                data_width--;
            }

            if(flags & NUM_XSIGN){
                *(*buf)++='0';
                *(*buf)++='x';
            }
        }
        for (i=data_precision - 1;i>=0;number /= R,i--)
            *((*buf)+i) = num_sys[number % R];
        *buf += data_precision;
        data_width -= data_precision;
    }
    return data_width;
}

int vsprintf(char *buf,const char * format,va_list ap){
    unsigned long flags;
    int data_width = 0;
    int data_precision = 0;

    while(*format != '\0'){
        if(*format != '%'){
            *buf++ = *format++;
            continue;
        }
        flags = 0;
REPEAT:
        format++;
        switch (*format) {
                case '-':
                    flags |= NUM_LEFT;
                    goto REPEAT;
                case '+':
                    flags |= NUM_SIGN;
                    goto REPEAT;
                case ' ':
                    flags |= NUM_SPACE;
                    goto REPEAT;
                case '#':
                    flags |= NUM_XSIGN;
                    goto REPEAT;
                case '0':
                    flags |= NUM_ZERO;
                    goto REPEAT;
        }
        data_width = todigital(&format);
        if(*format == '.'){
            format++;
            data_precision = todigital(&format);
        }
        switch(*format){
            case 'o':
                number(&buf, (unsigned long)va_arg(ap, int), flags, 8,data_width,data_precision);
                break;
            case 'i':
            case 'd':
                number(&buf, (unsigned long)va_arg(ap, int), flags | NUM_SIGN, 10,data_width,data_precision);
                break;
            case 'l':
                format++;
                switch (*format) {
                    case 'd':
                        number(&buf, (unsigned long)va_arg(ap, long), flags | NUM_SIGN, 10, data_width,data_precision);
                        break;
                }
                break;
            case 'x':
                number(&buf, (unsigned long)va_arg(ap, long), flags & ~NUM_SIGN, 16,data_width,data_precision);
                break;
            case 'X':
                number(&buf, (unsigned long)va_arg(ap, long), flags | NUM_CAPITAL & ~NUM_SIGN, 16,data_width,data_precision);
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
    }
    *buf++='\0';
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

    if(flags & ROLL_DOWN){
        destination = pos.vromaddr + (pos.height/pos.charys - lines)*pos.charys*pos.width;
        size = lines*pos.charys*pos.width*sizeof(int);
    }
    memset(destination,0,size);
}

void color_print(unsigned int bkcolor,unsigned int bgcolor,const char *format,...){
    va_list ap;
    char *buf = (char *)print_buf;
    int i;

    va_start(ap, format);
    vsprintf(buf,format,ap);
    va_end(ap);

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
            case '\e':
                buf++;
                if(*buf == '['){
                    buf++;
                    unsigned int tmp = 0;
                    if((*(long *)buf << 8) >>8 == *(long *)"0;38;2;"){
                        buf += 7;
                        tmp |= todigital((const char **)&buf) << 16;
                        if(*buf++ != ';')continue;
                        tmp |= todigital((const char **)&buf) << 8;
                        if(*buf++ != ';')continue;
                        tmp |= todigital((const char **)&buf);
                        if(*buf != 'm')continue;
                        bkcolor = tmp;
                    }else{
                        unsigned int x,y;
                        x = todigital((const char **)&buf);
                        if(*buf++ != ';')continue;
                        y = todigital((const char **)&buf);
                        if(*buf != 'H')continue;
                        if(x >= pos.width/pos.charxs)x=pos.width/pos.charxs-1;
                        if(y >= pos.height/pos.charys)y=pos.height/pos.charys-1;
                        pos.x = x;
                        pos.y = y;
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

void init_ppos(){
    register unsigned long vbe_mode_info_block_ptr = (unsigned long)PhyToVirtAddr(0x8200);
    pos.x=0;
    pos.y=0;
    pos.width=*(unsigned short *)(vbe_mode_info_block_ptr + 18);
    pos.height=*(unsigned short *)(vbe_mode_info_block_ptr + 20);
    pos.charxs=8;
    pos.charys=16;
    pos.vromaddr=(int *)((unsigned long)*(unsigned int *)(vbe_mode_info_block_ptr+40) + 0xffff800040000000);
    /* pos.vromaddr=(int *)0xffff800002200000; */
}
