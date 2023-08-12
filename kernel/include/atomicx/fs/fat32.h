#ifndef __FAT32_H
#define __FAT32_H

#include <atomicx/vfs.h>
#include <atomicx/disk.h>

#define FAT32_ATTR_READ_ONLY 0x01
#define FAT32_ATTR_HIDDEN 0x02
#define FAT32_ATTR_SYSTEM 0x04
#define FAT32_ATTR_VOLUME_ID 0x08
#define FAT32_ATTR_DIRECTORY 0x10
#define FAT32_ATTR_LONG_NAME 0x0f

#define FAT32_CLUS_TO_SEC(clus,sec_per_clus,data_start_sec) ((((clus) - 2)*(sec_per_clus))+(data_start_sec))

#define FAT32_OPEN_SUCCESS 0x00UL
#define FAT32_OPEN_ERR_NOT_FOUND 0x01UL

#define FAT32_LIST_OPT_ABSOLUTE_PATH 0x01UL

struct fat32_boot_sec{
    unsigned char BS_jmpBoot[3];
    unsigned char BS_OEMName[8];
    unsigned short BPB_BytesPerSec;
    unsigned char BPB_SecPerClus;
    unsigned short BPB_RsvdSecCnt;
    unsigned char BPB_NumFATs;
    unsigned short BPB_RootEntCnt;
    unsigned short BPB_TotSec16;
    unsigned char BPB_Media;
    unsigned short BPB_FATSz16;
    unsigned short BPB_SecPerTrk;
    unsigned short BPB_NumHeads;
    unsigned int BPB_HiddSec;
    unsigned int BPB_TotSec32;

    //only in fat32
    unsigned int BPB_FATSz32;
    unsigned short BPB_ExtFlags;
    unsigned short BPB_FSVer;
    unsigned int BPB_RootClus;
    unsigned short BPB_FSInfo;
    unsigned short BPB_BkBootSec;
    unsigned char BPB_Reserved[12];

    unsigned char BS_DrvNum;
    unsigned char BS_Reservd1;
    unsigned char BS_BootSig;
    unsigned int BS_VolID;
    unsigned char BS_VolLab[11];
    unsigned long BS_FillSysType;

    unsigned char Boot_Code[420];
    unsigned char EndSig[2];
}__attribute__((packed));


struct fat32_fs_info{
    unsigned int FSI_LeadSig;
    unsigned char FSI_Reserved1[480];
    unsigned int FSI_StrucSig;
    unsigned int FSI_Free_Count;
    unsigned int FSI_Nxt_Free;
    unsigned char FSI_Reserved2[12];
    unsigned int FSI_TrailSig;
}__attribute__((packed));

struct fat32_short_directory{
    unsigned char DIR_name[11];
    unsigned char DIR_Attr;
    unsigned char DIR_NTRes;
    unsigned char DIR_CrtTimeTenth;
    unsigned short DIR_CrtTime;
    unsigned short DIR_CrtDate;
    unsigned short DIR_LastAccDate;
    unsigned short DIR_FstClusHI;
    unsigned short DIR_WrtTime;
    unsigned short DIR_WrtDate;
    unsigned short DIR_FstClusLO;
    unsigned int DIR_FileSize;
}__attribute__((packed));

struct fat32_long_directory{
    unsigned char LDIR_Ord;
    unsigned char LDIR_Name1[10];
    unsigned char LDIR_Attr;
    unsigned char LDIR_Type;
    unsigned char LDIR_Chksum;
    unsigned char LDIR_Name2[12];
}__attribute__((packed));

unsigned long fat32_open(struct disk_partition *fat32_part,struct vfs_fd * fd,const char * path,unsigned long start_directory_sec);
const char ** fat32_list(struct disk_partition *fat32_part,struct vfs_fd *fd,unsigned long option);
#endif
