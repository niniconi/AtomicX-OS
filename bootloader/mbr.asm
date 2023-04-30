;======================================
;mbr 扇区的加载boot.bin 的程序
;适用文件系统FAT32
;======================================
org 0x7c00
%include "fat32.asm"

fat32SIG equ 0x0c
partTable equ 0x7dbe
fat32Buf equ 0x7e00
buf equ 0x8000
bootaddr equ 0x8200

jmp start
%include "util.asm"

start:

; clear the screen
mov ah,0x00
mov al,0x03
int 0x10

mov si,partTable
mov cx,4
findActPart:
    cmp byte [si],0x80
    je checkFs
    add si,0x10
    loop findActPart
    mov cx,NoActive_len
    mov bp,NoActive
    call error
    jmp $
checkFs:
    cmp byte [si+0x04],fat32SIG
    je getRootDir
    mov cx,NoFat32_len
    mov bp,NoFat32
    call error
    jmp $
getRootDir:
    mov cx,YesFat32_len
    mov bp,YesFat32
    call info

    mov eax,[si+0x08]
    mov word [sectorCnt],0x01
    mov dword [bufaddr],fat32Buf
    mov dword [startSector],eax
    call read

    xor eax,eax
    xor ebx,ebx
    xor ecx,ecx
    mov cx,[fat32Buf+BPB_ResvdSecCnt]
    mov al,[fat32Buf+BPB_NumFATs]
    mov bx,[fat32Buf+BPB_FATSz16]

    cmp bx,0x00
    jne FATSz16
    mov ebx,[fat32Buf+BPB_FATSz32]
    mul ebx
    jmp getDataSec
    FATSz16:
    mul bx

    getDataSec:
    add ecx,eax
    mov ebx,[fat32Buf+PBP_HiddSec]
    add ecx,ebx
    push ecx

    mov word [sectorCnt],0x01
    mov dword [bufaddr],buf
    mov dword [startSector],ecx
    call read

    mov cx,512/32
    mov si,buf
lookupBoot:
    cmp dword [si],"BOOT"
    jne next
    cmp dword [si+4],"    "
    jne next
    cmp dword [si+7]," BIN"
    jne next

    mov bx,[si+DIR_FstClusLO]
    mov ax,[si+DIR_FstClusHI]
    shl ax,16
    or eax,ebx
    sub eax,2

    xor ebx,ebx
    mov bl,[fat32Buf+BPB_SecPerClus]
    mul ebx
    pop ecx
    add eax,ecx
    mov dword [startSector],eax

    mov eax,[si+DIR_FileSize]
    mov ebx,512
    div ebx
    cmp edx,0
    je NoRemainder
    inc eax
    NoRemainder:
    mov word [sectorCnt],ax
    mov dword [bufaddr],bootaddr
    jmp load

    next:
    add si,32
    loop lookupBoot
    mov cx,NotFoundBoot_len
    mov bp,NotFoundBoot
    call error
    jmp $

    load:
    call read

    mov cx,Success_len
    mov bp,Success
    call info
    jmp bootaddr

message:
    NoActive db 'err0'
    NoActive_len equ $-NoActive
    NoFat32 db 'err1'
    NoFat32_len equ $-NoFat32
    NotFoundBoot db 'err2'
    NotFoundBoot_len equ $-NotFoundBoot
    YesFat32 db 'it is fat32'
    YesFat32_len equ $-YesFat32
    Success db 'success'
    Success_len equ $-Success
