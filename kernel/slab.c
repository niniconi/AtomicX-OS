#include "slab.h"
#include "lib.h"

struct kmem_cache Cache_chain;

int init_slab(){
    memset(&Cache_chain,0,sizeof(struct kmem_cache));
}
