#ifndef __SLAB_H
#define __SLAB_H

struct task_struct{};
struct inode{};
struct slab{};

struct kmem_cache{
    struct kmem_cache * last;
    struct kmem_cache * next;
};

int init_slab();
#endif
