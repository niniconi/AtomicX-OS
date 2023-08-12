#ifndef __DATE_H
#define __DATE_H
#include "lib.h"

struct cmos_date{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char week;
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char century;
};

void get_cmos_date(struct cmos_date * date);

#endif
