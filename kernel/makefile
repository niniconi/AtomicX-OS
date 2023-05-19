OBJ=head.o kernel.o print.o
BIN=kernel.bin
DIR=../master

head.o:head.S
	gcc -E head.S > _head.S
	as --64 -o head.o _head.S
kernel.o:kernel.c
	gcc -mcmodel=large -fno-builtin -m64 -c kernel.c -o kernel.o
print.o:print.c
	gcc -mcmodel=large -fno-builtin -m64 -c print.c -o print.o

kernel.elf:$(OBJ)
	ld -b elf64-x86-64 -o kernel.elf $(OBJ) -T Kernel.lds
kernel.bin:kernel.elf
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary kernel.elf kernel.bin
install:kernel.bin
	cp ./kernel.bin $(DIR)/kernel.bin
clean:
	rm $(OBJ) $(BIN) kernel.elf _head.S -f
