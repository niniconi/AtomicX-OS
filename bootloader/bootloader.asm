;=================================
;加载内核文件 kernel.bin
;适用文件系统FAT32
;=================================
org 0x8200
jmp start
%include "fat32.asm"
%include "util.asm"


fat32Addr equ 0x7e00
rootDirAddr equ 0x8000

kernel equ 0x100000
kernelBuf equ 0x7e00
kernelBufLen equ 2

; Here's hardware info will be send to kernel.
e820MemoryStruct equ 0x7e00
vbeInfoBlock equ 0x8000
vbeModeInfoBlock equ 0x8200

[BITS 16]

start:
    ;clear the screen
    mov ah,0x00
    mov al,0x03
    int 0x10

openA20:
    in al,0x92
    or al,0b00000010
    out 0x92,al

    cli
    db 0x66
    lgdt [GDTPtr]


    mov eax,cr0
    or eax,1
    mov cr0,eax

    mov ax,0x10;load data segment descripter into gs
    mov gs,ax

    mov eax,cr0
    and eax,0xfffffffe
    mov cr0,eax
    sti

    mov cx,OpenedA20_len
    mov bp,OpenedA20
    call info

getStartDataSec:
    xor eax,eax
    xor ebx,ebx
    xor ecx,ecx
    mov cx,[fat32Addr+BPB_ResvdSecCnt]
    mov al,[fat32Addr+BPB_NumFATs]
    mov bx,[fat32Addr+BPB_FATSz16]

    cmp bx,0x00
    jne FATSz16
    mov ebx,[fat32Addr+BPB_FATSz32]
    mul ebx
    jmp getDataSec
    FATSz16:
    mul bx

    getDataSec:
    add ecx,eax
    mov ebx,[fat32Addr+PBP_HiddSec]
    add ecx,ebx
    push ecx

    mov cx,512/32
    mov si,rootDirAddr
lookupKernel:
    cmp dword [si],"KERN"
    jne next
    cmp dword [si+4],"EL  "
    jne next
    cmp dword [si+7]," BIN"
    jne next
    jmp kernelFileInfo
    next:
    add si,32
    loop lookupKernel
    mov cx,NoKernel_len
    mov bp,NoKernel
    call error
    jmp $

    kernelFileInfo:
    mov ax,[si+DIR_FstClusHI]
    shl eax,16
    mov ax,[si+DIR_FstClusLO]
    sub eax,2

    xor ebx,ebx
    mov bl,[fat32Addr+BPB_SecPerClus]
    mul ebx

    pop ecx
    add eax,ecx
    push eax
    ;eax is startSector

    mov eax,[si+DIR_FileSize]
    mov ebx,512
    div ebx
    cmp edx,0
    je NoRemainder0
    inc eax
    NoRemainder0:
    ;ax is sectorCnt

    mov cx,GetKernel_len
    mov bp,GetKernel
    call info
load:
    mov word [sectorCnt],kernelBufLen
    mov dword [bufaddr],kernelBuf
    mov bx,kernelBufLen
    mov dx,0
    div bx
    cmp dx,0
    je NoRemainder1
    inc ax
    NoRemainder1:
    mov cx,ax
    pop eax
    mov edi,kernel
    readKernel:
        mov dword [startSector],eax
        call read
        push cx
        mov ecx,kernelBufLen*512
        mov esi,kernelBuf
        movKernel:
            mov dl,[gs:esi]
            mov [gs:edi],dl
            inc edi
            inc esi
            loop movKernel
        pop cx
        add eax,kernelBufLen
        loop readKernel
    mov cx,HasLoadedKernel_len
    mov bp,HasLoadedKernel
    call info

getMemoryStruct:
    mov ebx,0x00000000
    mov di,e820MemoryStruct
    getMemoryStructLoop:
        mov ax,0x00
        mov es,ax
        mov eax,0x0000e820
        mov edx,0x534d4150
        mov ecx,20
        int 0x15
        jc failed
        add di,20
        cmp ebx,0x00000000
        jne getMemoryStructLoop
    jmp success
    mov DWORD [di],0x00
    mov DWORD [di+4],0x00
    mov DWORD [di+8],0x00
    mov DWORD [di+12],0x00
    mov DWORD [di+16],0x00
    failed:
        cmp ah,0x80
        jmp E820NotSupport
        cmp ah,0x86
        jmp E820InvalidCmd
        E820InvalidCmd:
        mov cx,E820FailedInvalid_len
        mov bp,E820FailedInvalid
        jmp callErrorE820
        E820NotSupport:
        mov cx,E820FailedNotSupport_len
        mov bp,E820FailedNotSupport
        callErrorE820:
        call error
        jmp $
    success:
        mov cx,E820Success_len
        mov bp,E820Success
        call info

getVBEInfo:
    mov ax,0x00
    mov es,ax
    mov di,vbeInfoBlock
    mov ax,0x4f00
    int 0x10

    cmp ax,0x004f
    jz displayVBEInfo
    mov cx,GetVBEInfoBlockFailed_len
    mov bp,GetVBEInfoBlockFailed
    call error
    jmp $

displayVBEInfo:
    mov ax,0x00
    mov es,ax
    mov di,vbeInfoBlock

    ; show VbeSignature
    mov bp,di
    mov cx,4
    call info

    ; mov esi,[vbeInfoBlock+14] ;get the video mode list ptr
; getVBEModeList:
    ; mov cx,word [esi]

    ; cmp cx,0xffff
    ; jz setVESA

    ; add esi,2

    ; mov ax,0x00
    ; mov es,ax
    ; mov di,vbeModeInfoBlock ; save to 0x8200
    ; mov ax,0x4f01
    ; int 0x10

    ; cmp ax,0x004f
    ; jz getVBEModeList

    ; mov cx,GetVBEModeListFailed_len
    ; mov bp,GetVBEModeListFailed
    ; call error
    ; jmp $

getSpecifiedVBEModeInfo:
    mov ax,0x00
    mov es,ax
    mov di,vbeModeInfoBlock; save the mode infomation to 0x8200
    mov ax,0x4f01
    mov cx,0x180
    int 0x10
    cmp ax,0x004f
    jz setVESA

    mov cx,GetVBEModeListFailed_len
    mov bp,GetVBEModeListFailed
    call error
    jmp $

setVESA:
    mov ax,0x4f02
    mov bx,0x4180 ;1440x900 bpp 32
    int 0x10

    cmp ax,0x004f
    jz intoProtectMode
    mov cx,SetVESAModeFailed_len
    mov bp,SetVESAModeFailed
    call error
    jmp $

intoProtectMode:
    cli
    db 0x66
    lgdt [GDTPtr]

    mov eax,cr0
    or eax,1
    mov cr0,eax

    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov fs,ax
    mov gs,ax
    mov esp,0x7c00

    jmp DWORD 0x08:intoLongMode
[BITS 32]
intoLongMode:
    checkSupportLongMode:
        mov eax,0x80000000
        cpuid
        cmp eax,0x80000001
        jb NotSupportLongModeInfo
        mov eax,0x80000001
        cpuid
        and edx,0x01 << 29
        cmp edx,0x01 << 29
        je SupportLongModeInfo
        NotSupportLongModeInfo:
            jmp $
    SupportLongModeInfo:
    chekcSupportPAE:
        mov eax,0x01
        cpuid
        and edx,0x01 << 6
        cmp edx,0x01 << 6
        je supportPAE
        jmp $
    supportPAE:

    mov dword [0x90000],0x91007
    mov dword [0x90800],0x91007

    mov dword [0x91000],0x92007

    mov dword [0x92000],0x000083
    mov dword [0x92008],0x200083
    mov dword [0x92010],0x400083
    mov dword [0x92018],0x600083
    mov dword [0x92020],0x800083
    mov dword [0x92020],0xa00083

    db 0x66
    lgdt [GDT64Ptr]
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    mov esp,0x7c00

    mov eax,cr4
    or eax,0x01 << 5
    mov cr4,eax

    mov eax,0x90000
    mov cr3,eax

    mov ecx,0xc0000080
    rdmsr
    bts eax,8
    wrmsr

    mov eax,cr0
    or eax,0x01 << 31
    mov cr0,eax

    jmp 0x08:kernel
message:
    NoKernel db "[error]:not found kernel.bin"
    NoKernel_len equ $-NoKernel
    GetVBEInfoBlockFailed db "[error]:get VBEInfoBlock Failed"
    GetVBEInfoBlockFailed_len equ $-GetVBEInfoBlockFailed
    GetVBEModeListFailed db "[error]:get Mode list failed"
    GetVBEModeListFailed_len equ $-GetVBEModeListFailed
    SetVESAModeFailed db "[error]:set SVGA mode(VESA VBE) failed"
    SetVESAModeFailed_len equ $-SetVESAModeFailed
    E820FailedInvalid db "[error]:get E820 struct failed(invalid command)"
    E820FailedInvalid_len equ $-E820FailedInvalid
    E820FailedNotSupport db "[error]:get E820 struct failed(not support)"
    E820FailedNotSupport_len equ $-E820FailedNotSupport
    E820Success db "[info ]:get E820 struct success"
    E820Success_len equ $-E820Success
    GetKernel db "[info ]:kernel.bin is exist and get kernel.bin info"
    GetKernel_len equ $-GetKernel
    HasLoadedKernel db "[info ]:has loaded kernel.bin"
    HasLoadedKernel_len equ $-HasLoadedKernel
    OpenedA20 db "[info ]:opened A20"
    OpenedA20_len equ $-OpenedA20

GDT:
    dq 0x0000000000000000;null segment
    dq 0x00cf9e000000ffff;code 32 bits segment
    dq 0x00cf92000000ffff;data 32 bits segment

GDTPtr:
    dw $-GDT
    dd GDT
GDT64:
    dq 0x0000000000000000;null segment
    dq 0x00209e0000000000;code 64 bits segment
    dq 0x0000920000000000;data 64 bits segment
GDT64Ptr:
    dw $-GDT64
    dd GDT64
