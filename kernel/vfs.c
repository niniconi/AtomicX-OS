#include <atomicx/memory.h>
#include <atomicx/print.h>
#include <atomicx/disk.h>
#include <atomicx/vfs.h>
#include <lib.h>

struct vfs *vfs = NULL;

enum fs_symbol{
    FS_FAT32 = 0x0c,
};


static void dentry_list_add(struct list * dentry,struct dentry * element){

}

int init_vfs(){
    vfs = kmalloc(0,sizeof(struct vfs));
    memset(vfs,0,sizeof(struct vfs));

    /*
     * read partition table from disk
     */
    unsigned char * mbr = kmalloc(0,512);
    struct disk_partition *part = NULL;
    int i;

    memset(mbr,0,512);
    disk_read(DISK_MASTER,1,0,mbr);

    for(i=0;i<4;i++){
        part = kmalloc(0,sizeof(struct disk_partition));
        memset(part,0,sizeof(struct disk_partition));
        if(*(unsigned char *)(mbr + 0x01be + i*16) == 0x80){
            part->active = true;
        }else if(*(unsigned char *)(mbr + 0x01be + i*16) == 0x00){
            part->active = false;

        }

        part->fs_type = *(unsigned char *)(mbr + 0x01be + i*16 + 0x04);
        part->start_lba = *(unsigned int *)(mbr + 0x01be + i*16 + 0x08);
        part->sec_cnt = *(unsigned int *)(mbr + 0x01be + i*16 + 0x0c);

        if(vfs->parts == NULL){
            vfs->parts = part;
        }else{
            vfs->parts->last = part;
            part->next = vfs->parts;
            vfs->parts = part;
        }
    }

    /*
     * init vfs file descriptor pool
     */

    struct file_descriptor_pool * fd_pool = kmalloc(0,sizeof(struct file_descriptor_pool));
    memset(fd_pool,0,sizeof(struct file_descriptor_pool));
    //TODO
    vfs->fd_pool = fd_pool;


    /*
     * build dentry tree start with root directory
     */
    struct dentry * root = kmalloc(0,sizeof(struct dentry));
    struct dentry * dev = kmalloc(0,sizeof(struct dentry));
    memset(root,0,sizeof(struct dentry));
    memset(dev,0,sizeof(struct dentry));

    root->d_name = "/";
    dev->d_name = "dev";

    dev->parent = root;

    dentry_list_add(root->chaild_dentry,dev);
    vfs->root = root;

    info("init vfs\n");
}

static struct file * fd_to_file(int fd){
    vfs->fd_pool;
}

int sys_open(const char * path,unsigned long option){

}

void sys_read(int fd,void * buf,unsigned long size){
    struct file *file = fd_to_file(fd);
}

void sys_write(int fd,void * buf,unsigned long size){
    struct file *file = fd_to_file(fd);
}

void sys_close(int fd){

}
