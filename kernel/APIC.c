#include <atomicx/print.h>
#include <atomicx/memory.h>
#include <atomicx/APIC.h>
#include <lib.h>

struct IOAPIC_mmap *ioapic_mmap = NULL;

void IOAPIC_write(unsigned char index,unsigned int data){
    *ioapic_mmap->io_index = index;
    mfence();
    *ioapic_mmap->io_data = data;
}

unsigned int IOAPIC_read(unsigned char index){
    *ioapic_mmap->io_index = index;
    mfence();
    return *ioapic_mmap->io_data;
}

void IOAPIC_rte_write(unsigned char index,unsigned long data){
    *ioapic_mmap->io_index = index;
    mfence();
    *ioapic_mmap->io_data = data & 0xffffffff;
    mfence();
    *ioapic_mmap->io_index = index+1;
    mfence();
    *ioapic_mmap->io_data = data >> 32;
}

unsigned long IOAPIC_rte_read(unsigned char index){
    unsigned long ret = 0;

    *ioapic_mmap->io_index = index;
    mfence();
    ret = *ioapic_mmap->io_data & 0xffffffff;
    mfence();
    *ioapic_mmap->io_index = index+1;
    mfence();
    ret |= ((unsigned long)*ioapic_mmap->io_data << 32);
    return ret;
}

int init_local_APIC(){
    unsigned long local_apic_id = rdmsr(0x802);
    unsigned long local_apic_version = rdmsr(0x803);
    info("local APIC ID %#018x\n",local_apic_id);
    info("local APIC version %#018x\n",local_apic_version);

    return 0;
}

int init_IO_APIC(){
    int i;

    // disable 8259a
    io_out8(0x21, 0xff);
    io_out8(0xa1, 0xff);

    unsigned long rte = 0;
    // set rte
    for(i=0;i<24;i++){
        rte = 0;

        rte = i + 0x20;
        rte |= IOAPIC_DELIVERY_MODE_FIXED << 8;
        IOAPIC_rte_write(2*i+0x10,rte);
    }

    return 0;
}

int init_APIC(){
    unsigned int a,b,c,d;
    cpuid(0x01, 0, &a, &b, &c, &d);

    if(d & (0x01 << 9)){
        info("support APIC\n");
    }else{
        warring("not support APIC, will init 8259a\n");
        return INIT_APIC_ERR_NOT_SUPPORT;
    }

    if(c & (0x01 << 21)){
        info("support X2APIC\n");
    }else{
        warring("not support X2APIC, will init_8259a\n");
        return INIT_APIC_ERR_NOT_SUPPORT;
    }

    // enable apic and x2apic
    __asm__ __volatile__(
            "movq $0x1b,%%rcx \n\t"
            "rdmsr \n\t"
            "bts $11,%%rax \n\t"
            "bts $10,%%rax \n\t"
            "wrmsr \n\t"
            :::);

    // enable svr software APIC
    __asm__ __volatile__(
            "movq $0x80f,%%rcx \n\t"
            "rdmsr \n\t"
            "bts $8,%%rax \n\t"
            "wrmsr \n\t"
            :::);

    // disalbe lvt
    __asm__ __volatile__(
            "movq $0x832,%%rcx \n\t"
            "rdmsr \n\t"
            "bts $16,%%rax\n\t"
            "wrmsr \n\t"
            "movq $0x833,%%rcx \n\t"
            "rdmsr \n\t"
            "bts $16,%%rax\n\t"
            "wrmsr \n\t"
            "movq $0x834,%%rcx \n\t"
            "rdmsr \n\t"
            "bts $16,%%rax\n\t"
            "wrmsr \n\t"
            "movq $0x835,%%rcx \n\t"
            "rdmsr \n\t"
            "bts $16,%%rax\n\t"
            "wrmsr \n\t"
            "movq $0x836,%%rcx \n\t"
            "rdmsr \n\t"
            "bts $16,%%rax\n\t"
            "wrmsr \n\t"
            "movq $0x837,%%rcx \n\t"
            "rdmsr \n\t"
            "bts $16,%%rax\n\t"
            "wrmsr \n\t"
            :::);


    ioapic_mmap = (struct IOAPIC_mmap *)kmalloc(0,sizeof(struct IOAPIC_mmap));
    memset(ioapic_mmap,0,sizeof(struct IOAPIC_mmap));

    ioapic_mmap->phy_addr = 0xfec00000UL;

    ioapic_mmap->io_index = (unsigned int *)PhyToVirtAddr(0xfec00000UL);
    ioapic_mmap->io_data = (unsigned int *)PhyToVirtAddr(0xfec00010UL);
    ioapic_mmap->eoi = (unsigned int *)PhyToVirtAddr(0xfec00040UL);

    init_local_APIC();
    init_IO_APIC();

    sti();

    info("init interrupt APIC\n");
    return 0;
}
