IMG=../master.img
DIR=../master

all:mbr.bin bootloader.bin
	dd if=mbr.bin of=$(IMG) bs=512 count=1 conv=notrunc

bootloader.bin:
	nasm -f bin bootloader.asm -o bootloader.bin
mbr.bin:
	nasm -f bin mbr.asm -o mbr.bin

clean:
	rm bootloader.bin mbr.bin -f
