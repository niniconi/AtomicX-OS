;=================================
;加载内核文件 kernel.bin
;适用文件系统FAT32
;=================================
org 0x8200
jmp start
%include "util.asm"

start:
    ;clear the screen
    mov ah,0x00
    mov al,0x03
    int 0x10

    mov cx,len
    mov bp,message
    call info
    jmp $
message:
    db 'this is bootloader'
    len equ $-message
