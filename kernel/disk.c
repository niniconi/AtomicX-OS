#include <atomicx/disk.h>
#include <atomicx/print.h>
#include <atomicx/interrupt.h>
#include <atomicx/memory.h>
#include <lib.h>

unsigned char disk_buf[512] = {0};
struct disk_driver disk_driver;

array_queue block_request_queue = INIT_ARRAY_QUEUE(128);

void init_disk(){
    //enable interrupt request
    io_out8(PIO_DISK_PORT_CTL_STAT, 0);
    io_out8(PIO_DISK_PORT_CTL_STAT & PIO_DISK_MASTER_TO_SLAVE, 0);

    memset(&disk_driver,0,sizeof(struct disk_driver));
    disk_driver.disk_buf = &disk_buf;

    disk_info(DISK_MASTER,&disk_driver.hd_info[0]);
    disk_info(DISK_SLAVE,&disk_driver.hd_info[1]);

    info("init disk\n");
}

void make_block_request(unsigned char hd_cmd,unsigned long size,unsigned char * data_buffer){
    struct block_device_request * request = kmalloc(0,sizeof(struct block_device_request));
    request->hd_cmd = hd_cmd;
    request->size = size;
    request->data_buffer = data_buffer;
    queue_put(&block_request_queue,(unsigned long)request);
}

void disk_handle(unsigned long nr){
    disk_wait();
    int i;
    struct block_device_request * request = (struct block_device_request *)queue_get(&block_request_queue);

    switch(request->hd_cmd){
        case HD_CMD_READ:
            for(i=0;i<request->size;i+=2){
                unsigned short data = io_in16(PIO_DISK_PORT_DATA);
                *(unsigned short *)(request->data_buffer+i) = data;
            }
            break;
        case HD_CMD_WRITE:
            for(i=0;i<request->size;i+=2);
            break;
        case HD_CMD_INFO:
            for(i=0;i<512;i+=2){
                unsigned short data = io_in16(PIO_DISK_PORT_DATA);
                *(unsigned short *)(request->data_buffer+i) = data;
            }
            break;
    }

    kfree(request);
}

int disk_read(unsigned int device,unsigned int sec_cnt,unsigned long lba,void * buffer){
    if (device == DISK_MASTER) {
        io_out8(PIO_DISK_PORT_SECTOR_CNT,sec_cnt);
        io_out8(PIO_DISK_PORT_ADDR0,lba & 0x000000ff);
        io_out8(PIO_DISK_PORT_ADDR1,(lba & 0x0000ff00) >> 8);
        io_out8(PIO_DISK_PORT_ADDR2,(lba & 0x00ff0000) >> 16);
        io_out8(PIO_DISK_PORT_CONF, PIO_DISK_CONF_LBA_MASTER | ( (lba & 0xff000000) >> 24));
        io_out8(PIO_DISK_PORT_CMD_STAT, ATA_CMD_RD_SECTOR);
    } else if(device == DISK_SLAVE) {
        io_out8(PIO_DISK_PORT_SECTOR_CNT & PIO_DISK_MASTER_TO_SLAVE ,sec_cnt);
        io_out8(PIO_DISK_PORT_ADDR0 & PIO_DISK_MASTER_TO_SLAVE ,lba & 0x000000ff);
        io_out8(PIO_DISK_PORT_ADDR1 & PIO_DISK_MASTER_TO_SLAVE ,(lba & 0x0000ff00) >> 8);
        io_out8(PIO_DISK_PORT_ADDR2 & PIO_DISK_MASTER_TO_SLAVE ,(lba & 0x00ff0000) >> 16);
        io_out8(PIO_DISK_PORT_CONF & PIO_DISK_MASTER_TO_SLAVE , PIO_DISK_CONF_LBA_MASTER | ( (lba & 0xff000000) >> 24) );
        io_out8(PIO_DISK_PORT_CMD_STAT & PIO_DISK_MASTER_TO_SLAVE , ATA_CMD_RD_SECTOR);
    }else return -1;
    make_block_request(HD_CMD_READ,sec_cnt * 512,buffer);
    int(0x2f);
    return sec_cnt * 512;
}

int disk_write(unsigned int device,unsigned long lba,void * buffer,unsigned long size){

}
void disk_info(unsigned int device,void * buffer){
    io_out8(PIO_DISK_PORT_CMD_STAT,ATA_CMD_DISK_INFO);
    make_block_request(HD_CMD_INFO,512,buffer);
    int(0x2f);
}
