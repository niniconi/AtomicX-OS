OBJ:=fat32.o
FLAGS:=-mcmodel=large -fno-builtin -fno-pic -m64
MACRO:=-DLOG_ERROR -DLOG_INFO -DLOG_WARRING -DLOG_DEBUG -DLOG_INTERRUPT -D_DEBUG
INCLUDE:=../include

build:$(OBJ)
fat32.o:fat32.c
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c fat32.c -o fat32.o -fno-stack-protector
clean:
	rm $(OBJ) -f
