#ifndef __IO_H
#define __IO_H

extern void inline io_out8(unsigned int port,unsigned char data){
    __asm__ __volatile__(
            "outb %%al,%%edx \n\t"
            :
            :"a"(data),"d"(port)
            :);
}

extern void inline io_out16(unsigned int port,unsigned short data){
    __asm__ __volatile__(
            "out %%ax,%%edx \n\t"
            :
            :"a"(data),"d"(port)
            :);
}

extern void inline io_out32(unsigned int port,unsigned int data){
    __asm__ __volatile__(
            "outl %%eax,%%edx\n\t"
            :
            :"a"(data),"d"(port)
            :);
}
extern unsigned char inline io_in8(unsigned int port){
    register unsigned char ret;
    __asm__ __volatile__(
            "inb %%edx,%%al \n\t"
            :"=a"(ret)
            :"d"(port)
            :);
    return ret;
}
extern unsigned short inline io_in16(unsigned int port){
    register unsigned short ret;
    __asm__ __volatile__(
            "in %%edx,%%ax \n\t"
            :"=a"(ret)
            :"d"(port)
            :);
    return ret;
}
extern unsigned int inline io_in32(unsigned int port){
    register unsigned int ret;
    __asm__ __volatile__(
            "inl %%edx,%%eax \n\t"
            :"=a"(ret)
            :"d"(port)
            :);
    return ret;
}

#endif
