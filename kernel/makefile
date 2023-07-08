OBJ=head.o entry.o kernel.o print.o interrupt.o trap.o keyboard.o disk.o
BIN=kernel.bin
DIR=../master
FLAGS:=-mcmodel=large -fno-builtin -fno-pic -m64
MACRO:=-DLOG_ERROR -DLOG_INFO -DLOG_WARRING -DLOG_DEBUG -DLOG_INTERRUPT -D_DEBUG

head.o:head.S
	gcc -E head.S > _head.S
	as --64 -o head.o _head.S
entry.o:entry.S
	gcc -E entry.S > _entry.S
	as --64 -o entry.o _entry.S
kernel.o:kernel.c
	gcc $(FLAGS) $(MACRO) -c kernel.c -o kernel.o
print.o:print.c
	gcc $(FLAGS) $(MACRO) -c print.c -o print.o -fno-stack-protector
interrupt.o:interrupt.c
	gcc $(FLAGS) $(MACRO) -c interrupt.c -o interrupt.o
trap.o:trap.c
	gcc $(FLAGS) $(MACRO) -c trap.c -o trap.o
keyboard.o:keyboard.c
	gcc $(FLAGS) $(MACRO) -DSCANCODE_VERSION=1 -c keyboard.c -o keyboard.o
disk.o:disk.c
	gcc $(FLAGS) $(MACRO) -c disk.c -o disk.o

kernel.elf:$(OBJ)
	ld -b elf64-x86-64 -z muldefs -o kernel.elf $(OBJ) -T Kernel.lds
kernel.bin:kernel.elf
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary kernel.elf kernel.bin
install:kernel.bin
	cp ./kernel.bin $(DIR)/kernel.bin
clean:
	rm $(OBJ) $(BIN) kernel.elf _head.S _entry.S -f
