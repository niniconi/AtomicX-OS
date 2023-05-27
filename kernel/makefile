OBJ=head.o kernel.o print.o entry.o interrupt.o trap.o
BIN=kernel.bin
DIR=../master
FLAGS:=-mcmodel=large -fno-builtin -m64

head.o:head.S
	gcc -E head.S > _head.S
	as --64 -o head.o _head.S
entry.o:entry.S
	gcc -E entry.S > _entry.S
	as --64 -o entry.o _entry.S
kernel.o:kernel.c
	gcc $(FLAGS) -c kernel.c -o kernel.o
print.o:print.c
	gcc $(FLAGS) -c print.c -o print.o -fno-stack-protector
interrupt.o:interrupt.c
	gcc $(FLAGS) -c interrupt.c -o interrupt.o
trap.o:trap.c
	gcc $(FLAGS) -c trap.c -o trap.o

kernel.elf:$(OBJ)
	ld -b elf64-x86-64 -z muldefs -o kernel.elf $(OBJ) -T Kernel.lds
kernel.bin:kernel.elf
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary kernel.elf kernel.bin
install:kernel.bin
	cp ./kernel.bin $(DIR)/kernel.bin
clean:
	rm $(OBJ) $(BIN) kernel.elf _head.S _entry.S -f
