error:
    push bx
    mov bl,0b00000100
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
printposition:
    x db 0x00
    y db 0x00

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
    test ah,ah
    jz ok
    mov cx,readFailed_len
    mov bp,readFailed
    call error
    ok:
    pop si
    pop ds
    pop dx
    pop ax
    ret
readMessage:
    readFailed db 'read err'
    readFailed_len equ $-readFailed

diskaddrpack:
    size             db 0x18
    reserve          db 0x00
    sectorCnt        dw 0x00
    bufaddr          dd 0x00
    startSector      dq 0x00
    ; bufaddrExtension dq 0x00
