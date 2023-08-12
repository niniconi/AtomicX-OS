#include <atomicx/slab.h>
#include <atomicx/print.h>
#include <lib.h>

extern struct memory_management memory_management;

struct slab_cache slab_caches[SLAB_CACHE_NR] =
{
    {.size=16,      .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=32,      .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=64,      .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=128,     .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=256,     .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=512,     .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=1024,    .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=2048,    .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=4096,    .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=8192,    .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=16384,   .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=32768,   .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=65536,   .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=131072,  .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=262144,  .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=524288,  .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0},
    {.size=1048576, .slab_full=NULL, .slab_partial=NULL, .slab_free=NULL, .used_cnt=0, .free_cnt=0, .slab_max_free=0}
};

int init_slab(){
    info("init slab\n");
}

struct slab * slab_create(unsigned long size){
    struct slab * slab = (struct slab *)memory_management.end;
    memset(slab,0,sizeof(struct slab));
    memory_management.end += sizeof(struct slab);

    unsigned long color_size = 0;
    unsigned int revd_color_map = 0;
    int i;

    switch(size){
        case 16:
        case 32:
        case 64:
        case 128:
        case 256:
        case 512:
        case 1024:
        case 2048:
        case 4096:
            slab->pages = alloc_pages(1,PAGEA_DIRECT,0);
            slab->page_count = 1;

            slab->color_length = DEFAULT_PAGE_SIZE/size;
            if(DEFAULT_PAGE_SIZE % size != 0)
                slab->color_length++;

            slab->color_map = (unsigned long *)slab->pages->virt_addr;
            color_size = slab->color_length/8;
            if(slab->color_length % 8 != 0)
                color_size++;
            memset(slab->color_map,0,color_size);

            slab->virt_addr = slab->pages->virt_addr+color_size;
            slab->size = DEFAULT_PAGE_SIZE;
            slab->free_size = DEFAULT_PAGE_SIZE - color_size;

            revd_color_map = color_size/size;
            if(color_size % size != 0)
                revd_color_map++;

            for(i=0;i<revd_color_map;i++){
                // TODO set color map
                /* slab->color_map |= ; */
            }
            break;
        case 8192:
        case 16384:
        case 32768:
        case 65536:
        case 131072:
        case 262144:
        case 524288:
        case 1048576:
            unsigned long page_cnt = 1048576/DEFAULT_PAGE_SIZE;
            if(1048576 % DEFAULT_PAGE_SIZE != 0) page_cnt++;

            slab->pages = alloc_pages(page_cnt,PAGEA_DIRECT,0);
            slab->page_count = page_cnt;

            slab->color_length = 1048576/size;
            if(DEFAULT_PAGE_SIZE % size != 0)
                slab->color_length++;

            slab->color_map = (unsigned long *)slab->pages->virt_addr;
            color_size = slab->color_length/8;
            if(slab->color_length % 8)
                color_size++;

            memset(slab->color_map,0,color_size);
            slab->virt_addr = slab->pages->virt_addr+color_size;
            slab->size = 1048576;
            slab->free_size = 1048576 - color_size;

            revd_color_map = color_size/size;
            if(color_size % size != 0)
                revd_color_map++;

            for(i=0;i<revd_color_map;i++){
                // TODO set color map
                /* slab->color_map |= ; */
            }
            break;
    }
    return slab;
}

int slab_delete(){}

/*
 * alloc memory for kernel.
 * in this kmalloc, will create a slab or use an existing slab
 */
void * kmalloc(unsigned long flags,unsigned long size){
    int i;
    struct slab_cache * slab_cache = NULL;

    /*
     * find the most fit slab cache.
     */
    for(i=0;i<SLAB_CACHE_NR;i++)
        if(size <= slab_caches[i].size) break;//TODO add check for rserved space of color map.

    slab_cache = &slab_caches[i];

    struct slab * slab = NULL;
    unsigned long alloc_virt_addr = NULL;

    if(size <= slab_cache->slab_max_free){
        /*
         * use the partial slab.
         */

        slab = slab_cache->slab_partial;
        while(slab != NULL){
            if(slab->max_continue_free_space = slab_cache->slab_max_free)
                break;

            slab=slab->next;
        }

        //TODO search free space by color map
        goto L1;
    }else{
L1:
        if(slab_cache->slab_free == NULL){
            /*
             * create new slab.
             */

            slab=slab_create(slab_cache->size);
            slab->free_size -= size;
            slab->max_continue_free_space = slab->free_size;

            //TODO full color map for alloc

            if(slab->free_size != 0){
                if(slab_cache->slab_partial == NULL) slab_cache->slab_partial = slab;
                else list_add_to_end(slab_cache->slab_partial,slab);

                if(slab_cache->slab_max_free < slab->max_continue_free_space)
                    slab_cache->slab_max_free = slab->max_continue_free_space;
            }else{
                if(slab_cache->slab_full == NULL) slab_cache->slab_full = slab;
                else list_add_to_end(slab_cache->slab_full,slab);
            }
            alloc_virt_addr = slab->virt_addr;
        }else{
            /*
             * use the freed slab.
             */

            slab = slab_cache->slab_free;
            slab_cache->slab_free = slab->next;
            if(slab->next != NULL)slab->next->last = NULL;

            slab->free_size -= size;
            slab->max_continue_free_space = slab->free_size;

            //TODO full color map for alloc

            if(slab->free_size != 0){
                if(slab_cache->slab_partial == NULL) slab_cache->slab_partial = slab;
                else list_add_to_end(slab_cache->slab_partial,slab);

                if(slab_cache->slab_max_free < slab->max_continue_free_space)
                    slab_cache->slab_max_free = slab->max_continue_free_space;
            }else{
                if(slab_cache->slab_full == NULL) slab_cache->slab_full = slab;
                else list_add_to_end(slab_cache->slab_full,slab);
            }

            alloc_virt_addr = slab->virt_addr;
        }
    }
    return (void *)alloc_virt_addr;
}


/*
 * free memory for kernel.
 */
void kfree(void * ptr){

}
