#include <atomicx/fs/fat32.h>
#include <atomicx/memory.h>
#include <atomicx/disk.h>
#include <atomicx/vfs.h>
#include <lib.h>

const char ** fat32_list(struct disk_partition *fat32_part,struct vfs_fd *fd,unsigned long option){
    if(fd->lba == 0 || fd->path == NULL)return NULL;

    int len = 0;
    int i,j;
    char ** path_list = NULL;
    char * path_buf = NULL;
    char * fd_path = fd->path;
    bool slash_in_end = false;

    while(*fd_path++ != '\0')len++;
    fd_path = fd->path;

    for(i = len;;i--){
        if((fd_path[i] | 0x20) <= 'z' && (fd_path[i] | 0x20) >= 'a'){
            slash_in_end = false;
            break;
        }else if(fd_path[i] == '/'){
            slash_in_end = true;
            break;
        }
    }

    if(fd->flags & VFS_FD_DIRECTORY){
        unsigned char * buf = kmalloc(0,512*2);
        struct fat32_short_directory *sd = (struct fat32_short_directory *)buf;

        path_list = kmalloc(0,sizeof(char *)*((512*2)/32));
        memset(path_list,0,sizeof(char *)*((512*2)/32));

        disk_read(fat32_part->device,2,fd->lba,buf);
        for(i=0,j=0;i<(512*2)/32;i++){
            if(sd->DIR_Attr & FAT32_ATTR_LONG_NAME){
                //#############debug##################
                sd = (struct fat32_short_directory *)((unsigned long)sd + 32);
                continue;
                //####################################
            }else{
                if(sd->DIR_name[0] == 0)break;
                if(slash_in_end){
                    path_list[j] = kmalloc(0,len+sizeof(char)*12);
                    memcpy(fd_path,path_list[j],len);
                    memcpy(sd->DIR_name,&path_list[j][len],sizeof(char)*11);
                    path_list[j][len+11] = '\0';
                }else{
                    path_list[j] = kmalloc(0,len+sizeof(char)*13);
                    memcpy(fd_path,path_list[j],len);
                    path_list[j][len] = '/';
                    memcpy(sd->DIR_name,&path_list[j][len+1],sizeof(char)*11);
                    path_list[j][len+12] = '\0';
                }

                sd = (struct fat32_short_directory *)((unsigned long)sd + 32);
                j++;
            }
        }
        kfree(buf);
    }else{
        path_list = kmalloc(0,sizeof(char *)*2);
        memset(path_list,0,sizeof(char *)*2);

        path_buf=kmalloc(0,len+1);
        memcpy(fd_path,path_buf,len+1);

        *path_list = path_buf;

    }
    return (const char **)path_list;
}

unsigned long fat32_open(struct disk_partition *fat32_part,struct vfs_fd * fd,const char * path,unsigned long start_directory_sec){
    unsigned long data_start_sec = 0;
    struct fat32_boot_sec * boot_sec = NULL;

    /*
     * start_directory_sec is used to specify the lba at the beginning of the query directory
     */
    if(start_directory_sec == 0){
        /*
         * get data area start(root directory sec) sector and get fat32 boot sector.
         */
        boot_sec = kmalloc(0,sizeof(struct fat32_boot_sec));

        disk_read(fat32_part->device,1,fat32_part->start_lba,boot_sec);
        if(boot_sec->BPB_FATSz16 == 0){
            data_start_sec = boot_sec->BPB_RsvdSecCnt + boot_sec->BPB_NumFATs*boot_sec->BPB_FATSz32 + boot_sec->BPB_HiddSec;
        }else{
            data_start_sec = boot_sec->BPB_RsvdSecCnt + boot_sec->BPB_NumFATs*boot_sec->BPB_FATSz16 + boot_sec->BPB_HiddSec;
        }
    }else{
        data_start_sec = start_directory_sec;
    }

    /*
     * find file or directory by path
     */
    const char *name = NULL;
    const char *ex_name = NULL;
    unsigned char * buf = NULL;
    struct fat32_short_directory * sd = NULL;
    struct fat32_long_directory * ld = NULL;
    unsigned int filename_length = 0;
    unsigned int fileexname_length = 0;
    unsigned int clus = 0;
    unsigned int file_size = 0;
    unsigned long target_sec = data_start_sec;
    bool is_end_in_path = false;
    int i;

    while(*path == '/')path++;
    if(*path == '\0'){
        fd->flags |= VFS_FD_DIRECTORY;
        fd->lba = data_start_sec;
        fd->size = 0;
        fd->crt_date = 0;//TODO get crt_date
        fd->chg_date = 0;//TODO get chg_date
        return FAT32_OPEN_SUCCESS;
    }

    buf=kmalloc(0,512);

    while(*path!= '\0'){
        disk_read(fat32_part->device,1,target_sec,buf);
        sd = (struct fat32_short_directory *)buf;

        /*
         * get file name(include file ex name) or directory name length.
         */
        name = path;
        for(filename_length=0;;filename_length++){

            if(*(name+filename_length)=='/'
                    || *(name+filename_length) == '\0'
                    || *(name+filename_length) == '.')break;

        }

        ex_name = NULL;
        fileexname_length = 0;
        if(*(name+filename_length) == '.'){
            ex_name=path+filename_length+1;

REGET_FILE_EX_NAME:
            for(fileexname_length=0;;fileexname_length++){
                if(*(ex_name+fileexname_length) == '.'){
                    filename_length+=fileexname_length+1;
                    ex_name = ex_name+fileexname_length+1;
                    goto REGET_FILE_EX_NAME;
                }

                if(*(ex_name+fileexname_length) == '/'
                        || *(ex_name+fileexname_length) == '\0')break;

            }
        }

        if(ex_name == NULL) i=filename_length;
        else i=filename_length+1+fileexname_length;

        is_end_in_path = true;
        for(;*(name + i) != '\0';i++){
            if((*(name + i) | 0x20) <= 'z' && (*(name + i) | 0x20) >= 'a'){
                is_end_in_path = false;
                break;
            }
        }

        if(!is_end_in_path)
            if(ex_name != NULL)filename_length += 1 + fileexname_length;

        /*
         * find file/dir in this directory
         */
        for(i=0;i<512/32;i++){
            if(sd->DIR_Attr & FAT32_ATTR_LONG_NAME){
                //#############debug##################
                sd = (struct fat32_short_directory *)((unsigned long)sd + 32);
                continue;
                //####################################

                unsigned int long_directory_ord = 0;
                ld = (struct fat32_long_directory *)sd;
                if(ld->LDIR_Ord & 0x40){
                    while(ld->LDIR_Attr & FAT32_ATTR_LONG_NAME){
                        long_directory_ord = ld->LDIR_Ord & ~0x40;
                        //TODO cmopare name by unicode.
                        ld=(struct fat32_long_directory *)((unsigned long)ld+32);
                    }
                    goto FIND_FILE_OR_DIR;
                }
            }else{
                if(strncmp(sd->DIR_name,name,filename_length) != 0 || strncmp(&sd->DIR_name[8],ex_name,fileexname_length) != 0){
                    sd = (struct fat32_short_directory *)((unsigned long)sd + 32);
                    continue;
                }else
                    goto FIND_FILE_OR_DIR;
            }
        }
        return FAT32_OPEN_ERR_NOT_FOUND;

FIND_FILE_OR_DIR:
        clus = (((unsigned int)sd->DIR_FstClusHI) << 16) + sd->DIR_FstClusLO;
        target_sec = FAT32_CLUS_TO_SEC(clus,boot_sec->BPB_SecPerClus,data_start_sec);
        if(is_end_in_path)break;
        if(sd->DIR_Attr & FAT32_ATTR_DIRECTORY){
            path+=filename_length;
            while(*path == '/')path++;
            continue;
        }else{
            return FAT32_OPEN_ERR_NOT_FOUND;
        }
    }

    /*
     * success find the file or directory.
     */
    if(sd->DIR_Attr & FAT32_ATTR_DIRECTORY) fd->flags |= VFS_FD_DIRECTORY;
    if(sd->DIR_Attr & FAT32_ATTR_READ_ONLY) fd->flags |= VFS_FD_READ_ONLY;
    fd->lba = target_sec;
    fd->size = sd->DIR_FileSize;
    fd->crt_date = 0;//TODO get crt_date
    fd->chg_date = 0;//TODO get chg_date

    kfree(buf);

    return FAT32_OPEN_SUCCESS;
}
