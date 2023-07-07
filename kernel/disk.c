#include "disk.h"
#include "lib.h"
#include "print.h"
#include "interrupt.h"

unsigned char disk_buf[512] = {0};

void init_disk(){
    //enable interrupt request
    io_out8(PIO_DISK_PORT_CTL_STAT, 0);
    io_out8(PIO_DISK_PORT_CTL_STAT & PIO_DISK_MASTER_TO_SLAVE, 0);

    info("init disk\n");
}

void disk_handle(unsigned long nr){
    disk_wait();
    int i;

    /* info("read from disk"); */
    for(i=0;i<512;i+=2){
        unsigned short data = io_in16(PIO_DISK_PORT_DATA);
        *(unsigned short *)&disk_buf[i] = data;
        /* if(i % 16 == 0)color_print(COL_WHITE,COL_BLACK,"\n"); */
        /* color_print(COL_RED, COL_BLACK, "%#04x ", disk_buf[i]); */
        /* color_print(COL_RED, COL_BLACK, "%#04x ", disk_buf[i+1]); */
    }
    /* color_print(COL_WHITE,COL_BLACK,"\n"); */
}

int disk_read(int device,int cnt,long addr){
    if (device == DISK_MASTER) {
        io_out8(PIO_DISK_PORT_SECTOR_CNT,cnt);
        io_out8(PIO_DISK_PORT_ADDR0,addr & 0x000000ff);
        io_out8(PIO_DISK_PORT_ADDR1,(addr & 0x0000ff00) >> 8);
        io_out8(PIO_DISK_PORT_ADDR2,(addr & 0x00ff0000) >> 16);
        io_out8(PIO_DISK_PORT_CONF, PIO_DISK_CONF_LBA_MASTER | (addr & (0xff000000) >> 24) );
        io_out8(PIO_DISK_PORT_CMD_STAT, ATA_CMD_RD_SECTOR);
    } else if(device == DISK_SLAVE) {
        io_out8(PIO_DISK_PORT_SECTOR_CNT & PIO_DISK_MASTER_TO_SLAVE ,cnt);
        io_out8(PIO_DISK_PORT_ADDR0 & PIO_DISK_MASTER_TO_SLAVE ,addr & 0x000000ff);
        io_out8(PIO_DISK_PORT_ADDR1 & PIO_DISK_MASTER_TO_SLAVE ,(addr & 0x0000ff00) >> 8);
        io_out8(PIO_DISK_PORT_ADDR2 & PIO_DISK_MASTER_TO_SLAVE ,(addr & 0x00ff0000) >> 16);
        io_out8(PIO_DISK_PORT_CONF & PIO_DISK_MASTER_TO_SLAVE , PIO_DISK_CONF_LBA_MASTER | ((addr & 0xff000000) >> 24) );
        io_out8(PIO_DISK_PORT_CMD_STAT & PIO_DISK_MASTER_TO_SLAVE , ATA_CMD_RD_SECTOR);
    }else return -1;
    int(0x2f);
    return cnt * 512;
}

int disk_write(int device){

}
void disk_info(int device){
    io_out8(PIO_DISK_PORT_CMD_STAT,ATA_CMD_DISK_INFO);
    int(0x2f);
}
