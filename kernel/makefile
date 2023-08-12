export FLAGS:=-mcmodel=large -fno-builtin -fno-pic -m64
export MACRO:=-DLOG_ERROR -DLOG_INFO -DLOG_WARRING -DLOG_DEBUG -DLOG_INTERRUPT -D_DEBUG

FILESYSTEM:=fs/fat32.o
INCLUDE:=./include
BIN:=kernel.bin
DIR:=../master
OBJ:=head.o entry.o kernel.o print.o interrupt.o trap.o keyboard.o disk.o memory.o slab.o APIC.o 8259a.o date.o vfs.o
# PIC:=-DAPIC
PIC:=

build:$(BIN)

install:build
	cp ./kernel.bin $(DIR)/kernel.bin

head.o:head.S clean
	gcc -E head.S > _head.S
	as --64 -o head.o _head.S
entry.o:entry.S clean
	gcc -E entry.S > _entry.S
	as --64 -o entry.o _entry.S
kernel.o:kernel.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c kernel.c -o kernel.o
print.o:print.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c print.c -o print.o -fno-stack-protector
interrupt.o:interrupt.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) $(PIC) -c interrupt.c -o interrupt.o
trap.o:trap.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c trap.c -o trap.o
keyboard.o:keyboard.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -DSCANCODE_VERSION=1 -c keyboard.c -o keyboard.o
disk.o:disk.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c disk.c -o disk.o
memory.o:memory.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c memory.c -o memory.o
slab.o:slab.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c slab.c -o slab.o
APIC.o:APIC.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c APIC.c -o APIC.o -fno-stack-protector
8259a.o:8259a.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c 8259a.c -o 8259a.o
date.o:date.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c date.c -o date.o
vfs.o:vfs.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c vfs.c -o vfs.o
filesystem:
	$(MAKE) -C fs

kernel.elf:$(OBJ) filesystem
	ld -b elf64-x86-64 -z muldefs -o kernel.elf $(OBJ) $(FILESYSTEM) -T Kernel.lds
kernel.bin:kernel.elf
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary kernel.elf kernel.bin

clean:
	rm $(OBJ) $(BIN) $(FILESYSTEM) kernel.elf _head.S _entry.S -f
	$(MAKE) -C fs clean
