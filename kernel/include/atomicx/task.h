#ifndef __TASK_H
#define __TASK_H

struct tss_struct{
    unsigned int reserved0;
    unsigned long rsp0;
    unsigned long rsp1;
    unsigned long rsp2;
    unsigned long reserved1;
    unsigned long ist1;
    unsigned long ist2;
    unsigned long ist3;
    unsigned long ist4;
    unsigned long ist5;
    unsigned long ist6;
    unsigned long ist7;
    unsigned long reserved2;
    unsigned short reserved3;
    unsigned short iomapbaseaddr;
}__attribute__((packed));

struct task_mm_struct{
    void * page_table;

    unsigned long start_code,end_code;
    unsigned long start_data,end_data;
    unsigned long start_rodata,end_rodata;
    unsigned long start_bss,end_bss;

    unsigned long start_brk,end_brk;
    unsigned long start_stack;
};

struct thread{
    unsigned long rip;
    unsigned long rsp;
    unsigned long cs;
    unsigned long ds;

    unsigned long cr2;
};

struct task{
    struct list * list;

    unsigned long pid;

    struct task_mm_struct * mm_struct;
    struct thread * thread;

    unsigned long addr_limit;

    unsigned long stat;

    unsigned long flags;
};

union task_union{
    struct task task;
    unsigned char space[32768];
};
#endif
