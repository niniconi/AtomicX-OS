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
     * base file
     */
    struct file * f_stdout = kmalloc(0,sizeof(struct file));
    struct file * f_stdin = kmalloc(0,sizeof(struct file));
    struct file * f_stderr = kmalloc(0,sizeof(struct file));
    memset(f_stdout,0,sizeof(struct file));
    memset(f_stdin,0,sizeof(struct file));
    memset(f_stderr,0,sizeof(struct file));
    f_stdout->name = "stdout";
    f_stdin->name = "stdin";
    f_stderr->name = "stderr";

    /*
     * init vfs file descriptor pool
     */

    struct file_descriptor_pool * fd_pool = kmalloc(0,sizeof(struct file_descriptor_pool));
    struct node * node = NULL;
    struct fd * s_fd = NULL;

    memset(fd_pool,0,sizeof(struct file_descriptor_pool));

    node = kmalloc(0,sizeof(struct node));
    s_fd = kmalloc(0,sizeof(struct fd));
    memset(node,0,sizeof(struct node));
    memset(s_fd,0,sizeof(struct fd));
    s_fd->fd = stdout;
    s_fd->file = f_stdout;
    node->data = (unsigned long)s_fd;
    fd_pool->fds = node;

    node = kmalloc(0,sizeof(struct node));
    s_fd = kmalloc(0,sizeof(struct fd));
    memset(node,0,sizeof(struct node));
    memset(s_fd,0,sizeof(struct fd));
    s_fd->fd = stdin;
    s_fd->file = f_stdin;
    node->data = (unsigned long)s_fd;
    fd_pool->fds->right = node;

    node = kmalloc(0,sizeof(struct node));
    s_fd = kmalloc(0,sizeof(struct fd));
    memset(node,0,sizeof(struct node));
    memset(s_fd,0,sizeof(struct fd));
    s_fd->fd = stderr;
    s_fd->file = f_stderr;
    fd_pool->fds->right->right = node;

    fd_pool->fd_count+=3;

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

static int fd_pool_add(struct file * file,unsigned int flags){
    struct file_descriptor_pool *fd_pool = vfs->fd_pool;
    struct fd *s_fd = NULL;
    struct node * node = NULL;
    int fd;

    //TODO set fd ,must not equal with exist fd

    if(file == NULL)return -1;

    if(fd_pool->fds == NULL){
        s_fd = kmalloc(0,sizeof(struct fd));
        node = kmalloc(0,sizeof(struct node));
        memset(s_fd,0,sizeof(struct fd));
        memset(node,0,sizeof(struct node));

        node->data = (unsigned long)s_fd;
        fd_pool->fds= node;
        goto SET_FD;
    }else{
        node = fd_pool->fds;
        while(node != NULL){
            s_fd = (struct fd *)node->data;
            if(s_fd->fd > fd){
                if(node->left == NULL){
                    s_fd = kmalloc(0,sizeof(struct fd));
                    node->left = kmalloc(0,sizeof(node));
                    memset(node->left,0,sizeof(node));
                    memset(s_fd,0,sizeof(s_fd));

                    node->left->parent = node;
                    node->data = (unsigned long)s_fd;
                    goto SET_FD;
                }
                node = node->left;
            }else{
                if(node->right == NULL){
                    s_fd = kmalloc(0,sizeof(struct fd));
                    node->right= kmalloc(0,sizeof(node));
                    memset(node->right,0,sizeof(node));
                    memset(s_fd,0,sizeof(s_fd));

                    node->right->parent = node;
                    node->data = (unsigned long)s_fd;
                    goto SET_FD;
                }
                node = node->right;
            }
        }
    }
SET_FD:
    s_fd->fd = fd;
    s_fd->file = file;
    s_fd->flags = flags;

    fd_pool->fd_count++;
}

static struct file * fd_pool_get(int fd){
    struct file_descriptor_pool *fd_pool = vfs->fd_pool;
    struct fd * s_fd = NULL;
    struct node * node = NULL;

    if(fd_pool->fds == NULL){
        goto NOT_FOUND;
    }else{
        node = fd_pool->fds;
        while(node != NULL){
            s_fd = (struct fd *)node->data;
            if(s_fd->fd > fd){
                node = node->left;
            }else if(s_fd->fd == fd){
                return s_fd->file;
            }else{
                node = node->left;
            }
        }
    }

NOT_FOUND:
    return NULL;
}

static void fd_pool_del(int fd){
    struct file_descriptor_pool *fd_pool = vfs->fd_pool;
    struct fd * s_fd = NULL;
    struct node * node = NULL;

    //TODO delete fd from fd_pool
}

int sys_open(const char * path,unsigned int oflag){
    struct dentry * dir = vfs->root;
    struct file * file = NULL;
    const char * filename = NULL;
    unsigned int filename_length = 0;

    while(*path != '\0'){
        switch(*path){
            case '/':
                if(filename != NULL){
                    filename = NULL;
                    filename_length=0;
                    //TODO search file by dentry
                }else{
                    filename = path + 1;
                }
                continue;
            case 'a'...'z':
            case 'A'...'Z':
                filename_length++;
                break;
            default:
                goto OPEN_FAILED;
        }
    }

    int fd = fd_pool_add(file,oflag);
    if(fd != -1)
        return fd;

OPEN_FAILED:
}

void sys_read(int fd,void * buf,unsigned long size){
    struct file *file = fd_pool_get(fd);
}

void sys_write(int fd,void * buf,unsigned long size){
    struct file *file = fd_pool_get(fd);
}

void sys_close(int fd){
    fd_pool_del(fd);
}
