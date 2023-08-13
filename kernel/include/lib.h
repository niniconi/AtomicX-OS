#ifndef __LIB_H
#define __LIB_H

#include "lib/bool.h"
#include "lib/io.h"

#define NULL 0
#define nop() __asm__ __volatile__("nop \n\t":::)
#define cli() __asm__ __volatile__("cli \n\t":::)
#define sti() __asm__ __volatile__("sti \n\t":::)
#define mfence() __asm__ __volatile__("mfence \n\t":::)
#define int(nr) __asm__ __volatile__("int %0 \n\t"::"i"(nr):)

#define get_CR3(var) __asm__ __volatile__("movq %%cr3,%%rax \n\t":"=a"(var)::)

#define flush_page_table() \
    __asm__ __volatile__( \
            "movq %%cr3,%%rax \n\t" \
            "movq %%rax,%%cr3 \n\t" \
            :::)

#define set_page_table(page_addr) \
    __asm__ __volatile__("movq %%rax,%%cr3 \n\t"::"a"(page_addr):)

extern void inline wrmsr(unsigned int addr,unsigned long data){
    __asm__ __volatile__("wrmsr \n\t"::"c"(addr),"d"(data >> 32),"a"(data & 0xffffffff):);
}

extern unsigned long inline rdmsr(unsigned int addr){
    register unsigned long high;
    register unsigned long low;
    __asm__ __volatile__("rdmsr \n\t":"=d"(high),"=a"(low):"c"(addr):);
    return (high << 32) + (low & 0xffffffff);
}

extern void inline cpuid(unsigned int fn,unsigned int exfn,unsigned int *areg,unsigned int *breg,unsigned int *creg,unsigned int *dreg){
    __asm__ __volatile__(
            "cpuid \n\t"
            :"=a"(*areg),"=b"(*breg),"=c"(*creg),"=d"(*dreg)
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

extern int inline strncmp(const char * lstr,const char *rstr,unsigned long count){
    register int ret = 0;
    __asm__ __volatile__(
            "xorq %%rdx,%%rdx \n\t"
            "cmpq $0x00,%%rcx \n\t"
            "jz __strncmp_end \n\t"
            "__strncmp: \n\t"
            "xorq %%rax,%%rax \n\t"
            "xorq %%rbx,%%rbx \n\t"
            "movb 0x00(%%rsi),%%al \n\t"
            "movb 0x00(%%rdi),%%bl \n\t"
            "subq %%rax,%%rbx \n\t"
            "addq %%rbx,%%rdx \n\t"
            "addq $0x01,%%rsi \n\t"
            "addq $0x01,%%rdi \n\t"
            "loop __strncmp \n\t"
            "__strncmp_end: \n\t"
            :"=d"(ret)
            :"S"(rstr),"D"(lstr),"c"(count)
            :);
    return ret;
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
    unsigned long data[128];
}array_queue;

struct list{
    struct list * last;
    unsigned long data;
    struct list * next;
};

struct node{
    struct node * parent;

    unsigned long data;

    struct node * left;
    struct node * right;
};

#define def_forbid_intr_function(function) \
    __asm__(#function": \n\t" \
            "cli \n\t" \
            "call __"#function" \n\t" \
            "sti \n\t" \
            "ret \n\t")

extern unsigned long inline queue_get(array_queue * queue){
    if(queue->amount == 0) return 0;
    int index = queue->tail;
    if(index >= queue->length - 1)queue->tail=0;
    else queue->tail++;
    queue->amount--;
    return queue->data[index];
}
extern void inline queue_put(array_queue * queue,unsigned long data){
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
        while(current_node != NULL) { \
            if(current_node == node){ \
                if(current_node->last != NULL) { \
                    current_node->last->next = current_node->next; \
                } \
                break; \
            } \
            current_node = current_node->next; \
        } \
    }while(0)

#define list_to_end(node) while(node->next != NULL) node = node->next;


#endif
