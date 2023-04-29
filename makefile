master.img:
	dd if=/dev/zero of=master.img count=1 bs=512M
env:master.img
	fdisk master.img
	losetup --partscan --show --find master.img
	mkfs.fat -F32 /dev/loop0p1
	mkdir -p master
	mount /dev/loop0p1 ./master/
clean:
	losetup -d /dev/loop0
	umount /dev/loop0p1
	rm master -rf
	rm master.img -f
