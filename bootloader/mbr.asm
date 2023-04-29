;======================================
;mbr 扇区的加载bootloader.bin 的程序
;适用文件系统FAT32
;======================================
org 0x7c00

mov word  [sectorCnt],1
mov word  [bufaddr],0x7e00

read:
    push ax
    push dx
    push ds
    push si
    mov ax,0x00
    mov ds,ax
    mov si,diskaddrpack
    mov ah,0x42
    mov dl,0x80
    int 0x13
    pop si
    pop ds
    pop dx
    pop ax

jmp $

diskaddrpack:
    size             db 0x18
    reserve          db 0x00
    sectorCnt        dw 0x00
    bufaddr          dd 0x00
    startSector      dq 0x00
    ; bufaddrExtension dq 0x00
