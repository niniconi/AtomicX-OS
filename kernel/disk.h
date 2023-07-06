#ifndef __DISK_H
#define __DISK_H

#define ATA_CMD_DISK_INFO 0xec
#define ATA_CMD_RD_SECTOR 0x20
#define ATA_CMD_RD_SECTOR_EX 0x24
#define ATA_CMD_WR_SECTOR 0x30
#define ATA_CMD_WR_SECTOR_EX 0x34

/*
 * use IDE(PIO)
 * These MACRO default use master controler
 */
#define PIO_CMD_DATA 0x1f0
#define PIO_DISK_ADDR0 0x1f3
#define PIO_DISK_ADDR1 0x1f3
#define PIO_DISK_ADDR2 0x1f3
#define PIO_CTRL_STATUS_CMD 0x1f6

/*
 * This MACRO enables PIO to use slave controler.Just use the '&' with old MACRO.
 */
#define PIO_SLAVE_MASTER 0xff7f

/*
 */
#define PIO_CONF_CHS_MASTER 0xa0
#define PIO_CONF_LBA_MASTER 0xe0
#define PIO_CONF_CHS_SLAVE 0xb0
#define PIO_CONF_LBA_SLAVE 0xf0

#define PIO_CTRL_STATUS 0x3f6


void init_disk();

#endif
