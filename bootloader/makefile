IMG=../master.img
DIR=../master

install:mbr.bin bootloader.bin
	dd if=mbr.bin of=$(IMG) bs=512 count=1 conv=notrunc
	cp ./boot.bin  $(DIR)/boot.bin

bootloader.bin:
	nasm -f bin bootloader.asm -o boot.bin
mbr.bin:
	nasm -f bin mbr.asm -o mbr.bin

clean:
	rm boot.bin mbr.bin -f
