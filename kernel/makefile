OBJ=kernel.o head.o
BIN=kernel.bin
DIR=../master

head.o:head.S
	gcc -E head.S > _head.S
	as --64 -o head.o _head.S
kernel.o:kernel.c
	gcc -mcmodel=large -fno-builtin -m64 -c kernel.c
kernel.elf:kernel.o head.o
	ld -b elf64-x86-64 -o kernel.elf head.o kernel.o -T Kernel.lds
kernel.bin:kernel.elf
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary kernel.elf kernel.bin
install:kernel.bin
	cp ./kernel.bin $(DIR)/kernel.bin
clean:
	rm $(OBJ) $(BIN) kernel.elf _head.S -f
