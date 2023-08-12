#ifndef __SLAB_H
#define __SLAB_H
#include "memory.h"

#define SLAB_CACHE_NR 17

struct slab{
    struct page * pages;

    unsigned long virt_addr;
    unsigned long page_count;

    unsigned long free_size;
    unsigned long max_continue_free_space;
    unsigned long size;

    unsigned long color_length;
    unsigned long *color_map;

    struct slab * next;
    struct slab * last;
};

struct slab_cache{
    unsigned long size;

    unsigned long used_cnt;
    unsigned long free_cnt;

    unsigned long slab_max_free;

    struct slab * slab_partial;
    struct slab * slab_full;
    struct slab * slab_free;
};

int init_slab();
void * kmalloc(unsigned long flags,unsigned long size);
void kfree(void * ptr);
#endif
