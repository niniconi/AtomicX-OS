OBJ=head.o entry.o kernel.o print.o interrupt.o trap.o keyboard.o disk.o memory.o slab.o APIC.o 8259a.o date.o vfs.o
FILESYSTEM:=fs/fat32.o
BIN=kernel.bin
DIR=../master
FLAGS:=-mcmodel=large -fno-builtin -fno-pic -m64
MACRO:=-DLOG_ERROR -DLOG_INFO -DLOG_WARRING -DLOG_DEBUG -DLOG_INTERRUPT -D_DEBUG
INCLUDE:=./include
# PIC:=-DAPIC
PIC:=

build:clean $(BIN)

head.o:head.S
	gcc -E head.S > _head.S
	as --64 -o head.o _head.S
entry.o:entry.S
	gcc -E entry.S > _entry.S
	as --64 -o entry.o _entry.S
kernel.o:kernel.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c kernel.c -o kernel.o
print.o:print.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c print.c -o print.o -fno-stack-protector
interrupt.o:interrupt.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) $(PIC) -c interrupt.c -o interrupt.o
trap.o:trap.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c trap.c -o trap.o
keyboard.o:keyboard.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -DSCANCODE_VERSION=1 -c keyboard.c -o keyboard.o
disk.o:disk.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c disk.c -o disk.o
memory.o:memory.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c memory.c -o memory.o
slab.o:slab.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c slab.c -o slab.o
APIC.o:APIC.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c APIC.c -o APIC.o -fno-stack-protector
8259a.o:8259a.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c 8259a.c -o 8259a.o
date.o:date.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c date.c -o date.o
vfs.o:vfs.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c vfs.c -o vfs.o
filesystem:
	cd fs && $(MAKE) clean build

kernel.elf:$(OBJ) filesystem
	ld -b elf64-x86-64 -z muldefs -o kernel.elf $(OBJ) $(FILESYSTEM) -T Kernel.lds
kernel.bin:kernel.elf
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary kernel.elf kernel.bin
install:build
	cp ./kernel.bin $(DIR)/kernel.bin
clean:
	rm $(OBJ) $(BIN) $(FILESYSTEM) kernel.elf _head.S _entry.S -f
	cd fs && $(MAKE) clean
