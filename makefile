loop=/dev/loop0
# loop=/dev/block/loop0

run:bochsrc master.img
	bochs -q
master.img:
	dd if=/dev/zero of=master.img count=1 bs=512M
env:master.img
	fdisk master.img
	losetup --partscan --show --find master.img
	mkfs.fat -F32 $(loop)p1
	mkdir -p master
	mount $(loop)p1 ./master/
clean:
	losetup -d $(loop)
	umount $(loop)p1
	rm master -rf
	rm master.img -f
