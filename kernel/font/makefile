out:=mkfont font.h
kernel:=../

build:clean font.h

mkfont:mkfont.c
	gcc ./mkfont.c -o mkfont
font.h:mkfont
	./mkfont -i font.txt -o font.h
clean:
	rm $(out) -f
install:font.h
	cp ./font.h $(kernel)
