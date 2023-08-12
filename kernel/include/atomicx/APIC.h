#ifndef __APIC_H
#define __APIC_H

#define IOAPIC_DELIVERY_MODE_FIXED 0x000
#define IOAPIC_DELIVERY_MODE_LOWEST_PRIORITY 0x001
#define IOAPIC_DELIVERY_MODE_SMI 0x010
#define IOAPIC_DELIVERY_MODE_NMI 0x100
#define IOAPIC_DELIVERY_MODE_INIT 0x101
#define IOAPIC_DELIVERY_MODE_EXT_INT 0x111

#define INIT_APIC_ERR_NOT_SUPPORT 0x01

int init_APIC();

struct IOAPIC_mmap{
    unsigned long phy_addr;

    unsigned int * io_index;
    unsigned int * io_data;
    unsigned int * eoi;
};

#endif
