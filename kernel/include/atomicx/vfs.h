#ifndef __VFS_H
#define __VFS_H
#include <lib/bool.h>

#define VFS_FD_DIRECTORY 0x01L
#define VFS_FD_EXEC 0x02L
#define VFS_FD_READ_ONLY  0x04UL

struct vfs_fd{
    unsigned char * path;
    unsigned long flags;
    unsigned long lba;
    unsigned long size;
    unsigned long crt_date;
    unsigned long chg_date;
};

struct disk_partition{
    unsigned int device;
    unsigned long start_lba;
    unsigned long sec_cnt;
    unsigned char fs_type;
    bool active;

    struct disk_partition * next;
    struct disk_partition * last;
};

struct super_block{
    struct dentry *fs_root;
    struct super_block_options * s_ops;
};

struct file_options{
    unsigned long (*open)(struct super_block s_block);
    unsigned long (*read)(struct super_block s_block);
    unsigned long (*write)(struct super_block s_block);
};

struct dentry_options{
    const char ** (*list)(struct super_block s_block,unsigned long option);
};

struct super_block_options{

};

struct inode_options{

};

struct address_space{
    unsigned long lba;
    unsigned long size;
};


struct index_node{
    struct inode_options * i_ops;
    struct file_options * i_fops;
    struct address_space * addr_info;
};

struct file{
    struct dentry * dentry;
    struct file_options * f_ops;

    unsigned long flags;
    unsigned long size;
    unsigned long create_time;
    unsigned long update_time;
};

struct dentry{
    struct super_block * su_block;
    struct index_node * inode;

    struct file * file;

    const char * d_name;

    struct dentry_options d_ops;

    struct dentry * parent;
    struct list * chaild_dentry;
};

struct file_descriptor_pool{

};


struct vfs{
    struct disk_partition * parts;
    struct dentry * root;

    struct file_descriptor_pool * fd_pool;
};

enum open_ops{
    O_EXEC = 0x001,
    O_RDONLY = 0x002,
    O_RDWR = 0x004,
    O_SEARCH = 0x008,
    O_WRONLY = 0x010,
    O_APPEND = 0x020,
    O_CREAT = 0x040,
    O_DIRECTORY = 0x080,
    O_EXCL = 0x100,
    O_NOCTTY = 0x200,
    O_NOFOLLOW = 0x400,
    O_NONBLOCK = 0x800,
    O_TRUNC = 0x1000,
};

int init_vfs();
int sys_open(const char * path,unsigned int oflag);
void sys_read(int fd,void * buf,unsigned long size);
void sys_write(int fd,void * buf,unsigned long size);
void sys_close(int fd);
#endif
