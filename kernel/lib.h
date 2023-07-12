#ifndef __LIB_H
#define __LIB_H

#define NULL 0
#define nop() __asm__ __volatile__("nop \n\t":::)
#define cli() __asm__ __volatile__("cli \n\t":::)
#define sti() __asm__ __volatile__("sti \n\t":::)
#define int(nr) __asm__ __volatile__("int %0 \n\t"::"i"(nr):)

#define get_CR3(var) __asm__ __volatile__("movq %%cr3,%%rax \n\t":"=a"(var)::)

extern void inline cpuid(unsigned long fn,unsigned long exfn,unsigned int *areg,unsigned int *breg,unsigned int *creg,unsigned int *dreg){
    __asm__ __volatile__(
            "cpuid \n\t"
            :"=a"(*areg),"=b"(*breg),"=c"(creg),"=d"(dreg)
            :"a"(fn),"c"(exfn)
            :);
}

extern void inline memcpy(void * source,void *destination,unsigned long size){
    __asm__ __volatile__(
            "cld \n\t"
            "rep movsq \n\t"
            "movq %%rax,%%rcx \n\t"
            "rep movsb \n\t"
            :
            :"D"(destination),"S"(source),"c"(size/8),"a"(size % 8)
            :);
}
extern void inline memset(void * destination,unsigned char fb,unsigned long size){
    register unsigned long fq = fb;
    fq = fq | fq << 8 | fq << 16 | fq << 32;
    __asm__ __volatile__(
            "cld \n\t"
            "rep stosq \n\t"
            "movq %%rbx,%%rcx \n\t"
            "rep stosb \n\t"
            :
            :"D"(destination),"a"(fq),"c"(size/8),"b"(size % 8)
            :);
}

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
            "inb %%edx,%0 \n\t"
            :"=r"(ret)
            :"d"(port)
            :);
    return ret;
}
extern unsigned short inline io_in16(unsigned int port){
    register unsigned short ret;
    __asm__ __volatile__(
            "in %%edx,%0 \n\t"
            :"=r"(ret)
            :"d"(port)
            :);
    return ret;
}
extern unsigned int inline io_in32(unsigned int port){
    register unsigned int ret;
    __asm__ __volatile__(
            "inl %%edx,%0 \n\t"
            :"=r"(ret)
            :"d"(port)
            :);
}

typedef struct regs{
    unsigned long ds;
    unsigned long es;
    unsigned long rax;
    unsigned long rbx;
    unsigned long rcx;
    unsigned long rdx;
    unsigned long rbp;
    unsigned long rsi;
    unsigned long rdi;
    unsigned long r8;
    unsigned long r9;
    unsigned long r10;
    unsigned long r11;
    unsigned long r12;
    unsigned long r13;
    unsigned long r14;
    unsigned long r15;
}regs_t;

struct regsf{
    regs_t regs;
    unsigned long func; //reserved
    unsigned long errorcode;
    unsigned long rip;
    unsigned long cs;
    unsigned long rflags;
    unsigned long rsp;
    unsigned long ss;
}__attribute__((packed));
typedef struct regsf regsf_t;

#define INIT_ARRAY_QUEUE(len) { \
    .length = len, \
    .head =  len - 1, \
    .tail = 0, \
    .amount = 0 \
}

typedef struct array_queue{
    int head,tail;
    int amount;
    int length;
    char data[128];
}array_queue;

#define def_forbid_intr_function(function) \
    __asm__(#function": \n\t" \
            "cli \n\t" \
            "call __"#function" \n\t" \
            "sti \n\t" \
            "ret \n\t")

extern char inline queue_get(array_queue * queue){
    if(queue->amount == 0) return 0;
    int index = queue->tail;
    if(index >= queue->length - 1)queue->tail=0;
    else queue->tail++;
    queue->amount--;
    return queue->data[index];
}
extern void inline queue_put(array_queue * queue,char data){
    if(queue->amount == queue->length) return;
    if(queue->head >= queue->length - 1)queue->head = 0;
    else queue->head++;
    queue->data[queue->head] = data;
    queue->amount++;
}

#define list_add_to_end(node,content) \
    do{ \
        typeof(*node) *current_node = node; \
        while(current_node->next != NULL){ \
            current_node=current_node->next; \
        } \
        current_node->next=content; \
    }while(0)

#define list_add_to_start(node,content) \
    do { \
        typeof(*node) *current_node = node; \
        while(current_node->last != NULL){ \
            current_node=current_node->last; \
        } \
        current_node->last=content; \
    }while(0)

#define list_del_from_end(node) \
    do { \
        typeof(*node) *current_node = node; \
        while(current_node->next != NULL){ \
            current_node = current_node->next; \
        } \
        if(current_node->last != NULL){ \
            node->last->next = NULL; \
        } \
    }while(0)

#define list_del(root,node) \
    do { \
        typeof(*node) *current_node = root; \
        while(current_node->next != NULL) { \
            if(current_node == node){ \
                if(current_node->last != NULL) { \
                    current_node->last->next = current_node->next; \
                } \
                break; \
            } \
            current_node = current_node->next; \
        } \
    }while(0);

#endif
