#ifndef __DISK_H
#define __DISK_H

#define DISK_MASTER 0x01
#define DISK_SLAVE 0x02

#define ATA_CMD_DISK_INFO 0xec
#define ATA_CMD_RD_SECTOR 0x20
#define ATA_CMD_RD_SECTOR_EX 0x24
#define ATA_CMD_WR_SECTOR 0x30
#define ATA_CMD_WR_SECTOR_EX 0x34

/*
 * use IDE(PIO)
 * These MACRO default use master controler
 */
#define PIO_DISK_PORT_DATA 0x1f0
#define PIO_DISK_PORT_SECTOR_CNT 0x1f2
#define PIO_DISK_PORT_ADDR0 0x1f3
#define PIO_DISK_PORT_ADDR1 0x1f4
#define PIO_DISK_PORT_ADDR2 0x1f5
#define PIO_DISK_PORT_CONF 0x1f6
#define PIO_DISK_PORT_CMD_STAT 0x1f7
#define PIO_DISK_PORT_CTL_STAT 0x3f6

/*
 * This MACRO enables PIO to use slave controler.Just use the '&' with old MACRO.
 */
#define PIO_DISK_MASTER_TO_SLAVE 0xff7f

/*
 * These MACRO use in PIO_DISK_PORT_CONF
 */
#define PIO_DISK_CONF_CHS_MASTER 0xa0
#define PIO_DISK_CONF_LBA_MASTER 0xe0
#define PIO_DISK_CONF_CHS_SLAVE 0xb0
#define PIO_DISK_CONF_LBA_SLAVE 0xf0


#define disk_wait() while(io_in8(PIO_DISK_PORT_CMD_STAT) & 0x88 != 0x08)

void init_disk();
void disk_handle(unsigned long nr);

/*
 * return read/write length. if error then retrun value < 0 .
 * unit is byte.
 */
int disk_read(int device,int cnt,long addr);
int disk_write(int device);
void disk_info(int device);

#endif
