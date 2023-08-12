#include <atomicx/date.h>
#include <lib.h>

#define CMOS_READ(addr) ({ \
    io_out8(0x70,0x80 | (addr)); \
    io_in8(0x71); \
})

void get_cmos_date(struct cmos_date * date){
    unsigned char bcd = 0;

    bcd = CMOS_READ(0x00);
    date->second = (bcd & 0x0f) + 10*(bcd >> 4);
    bcd = CMOS_READ(0x02);
    date->minute = (bcd & 0x0f) + 10*(bcd >> 4);
    bcd = CMOS_READ(0x04);
    date->hour = (bcd & 0x0f) + 10*(bcd >> 4);
    bcd = CMOS_READ(0x06);
    date->week = (bcd & 0x0f) + 10*(bcd >> 4);
    bcd = CMOS_READ(0x07);
    date->day = (bcd & 0x0f) + 10*(bcd >> 4);
    bcd = CMOS_READ(0x08);
    date->month = (bcd & 0x0f) + 10*(bcd >> 4);
    bcd = CMOS_READ(0x09);
    date->year = (bcd & 0x0f) + 10*(bcd >> 4);
    bcd = CMOS_READ(0x32);
    date->century = (bcd & 0x0f) + 10*(bcd >> 4);

    io_out8(0x70,0x00);
}
