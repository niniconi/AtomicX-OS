;======================================
;mbr 扇区的加载bootloader.bin 的程序
;适用文件系统FAT32
;======================================
%include "fat32.asm"

org 0x7c00
partTable equ 0x7dbe
fat32Buf equ 0x7e00

mov si,partTable
mov cx,4
find:
    cmp byte [si],0x80
    je found
    add si,0x10
    loop find
    mov cx,NoActive_len
    mov bp,NoActive
    call error
    jmp $
found:
    cmp byte [si+0x04],0x0c
    je fat32
    mov cx,NoFat32_len
    mov bp,NoFat32
    call error
    jmp $
fat32:
    mov cx,Success_len
    mov bp,Success
    call info

    mov eax,[si+0x08]
    mov word [sectorCnt],0x01
    mov dword [bufaddr],fat32Buf
    mov dword [startSector],eax
    call read

    mov ax,[fat32Buf+BPB_ResvdSecCnt]
    mov cl,[fat32Buf+BPB_NumFATs]
    mov bx,[fat32Buf+BPB_FATSz16]
    cmp bx,0x00
    jne FATSz16
    mov ebx,[fat32Buf+BPB_FATSz32]
    FATSz16:


    jmp $
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
    ret
error:
    push bx
    mov bl,0b00000010
    jmp print
info:
    push bx
    mov bl,0b00000010
    jmp print
print:
    push ax
    push dx
    mov ax,0x00
    mov es,ax
    mov ah,0x13
    mov al,0x01
    mov dh,[y]
    mov dl,[x]
    mov bh,0x0
    int 0x10
    add byte [y],0x01
    pop dx
    pop ax
    pop bx
    ret

message:
    NoActive db 'err0x00'
    NoActive_len equ $-NoActive
    NoFat32 db 'err0x01'
    NoFat32_len equ $-NoFat32
    Success db 'success'
    Success_len equ $-Success

printposition:
    x db 0x00
    y db 0x00

diskaddrpack:
    size             db 0x18
    reserve          db 0x00
    sectorCnt        dw 0x00
    bufaddr          dd 0x00
    startSector      dq 0x00
    ; bufaddrExtension dq 0x00
