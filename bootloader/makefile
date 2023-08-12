IMG:=../master.img
DIR:=../master

build:clean mbr.bin bootloader.bin

install:build
	dd if=mbr.bin of=$(IMG) bs=512 count=1 conv=notrunc
	cp ./boot.bin  $(DIR)/boot.bin

bootloader.bin:
	nasm -f bin bootloader.asm -o boot.bin
mbr.bin:
	nasm -f bin mbr.asm -o mbr.bin

clean:
	rm boot.bin mbr.bin -f
