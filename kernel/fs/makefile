OBJ:=fat32.o
INCLUDE:=../include

build:$(OBJ)

fat32.o:fat32.c clean
	gcc $(FLAGS) $(MACRO) -I $(INCLUDE) -c fat32.c -o fat32.o -fno-stack-protector

clean:
	rm $(OBJ) -f
