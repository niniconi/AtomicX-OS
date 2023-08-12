#include <atomicx/keyboard.h>
#include <atomicx/print.h>
#include <lib.h>

#ifndef SCANCODE_VERSION
#error "Please define SCANCODE_VERSION,range is [1,2,3]"
#endif

array_queue keyboard_buf = INIT_ARRAY_QUEUE(128);

struct keyboard_stat kb_stat = INIT_KB_STAT();

unsigned char scancode_map[NR_SCANCODE] = {
/* 0000 */ 0x00,
/* 0x01 */ 0x00,
/* 0x02 */ '1',
/* 0x03 */ '2',
/* 0x04 */ '3',
/* 0x05 */ '4',
/* 0x06 */ '5',
/* 0x07 */ '6',
/* 0x08 */ '7',
/* 0x09 */ '8',
/* 0x0a */ '9',
/* 0x0b */ '0',
/* 0x0c */ '-',
/* 0x0d */ '=',
/* 0x0e */ 0x00,
/* 0x0f */ 0x00,
/* 0x10 */ 'Q',
/* 0x11 */ 'W',
/* 0x12 */ 'E',
/* 0x13 */ 'R',
/* 0x14 */ 'T',
/* 0x15 */ 'Y',
/* 0x16 */ 'U',
/* 0x17 */ 'I',
/* 0x18 */ 'O',
/* 0x19 */ 'P',
/* 0x1a */ '[',
/* 0x1b */ ']',
/* 0x1c */ 0x00,
/* 0x1d */ 0x00,
/* 0x1e */ 'A',
/* 0x1f */ 'S',
/* 0x20 */ 'D',
/* 0x21 */ 'F',
/* 0x22 */ 'G',
/* 0x23 */ 'H',
/* 0x24 */ 'J',
/* 0x25 */ 'K',
/* 0x26 */ 'L',
/* 0x27 */ ';',
/* 0x28 */ '\'',
/* 0x29 */ '`',
/* 0x2a */ 0x00,
/* 0x2b */ '\\',
/* 0x2c */ 'Z',
/* 0x2d */ 'X',
/* 0x2e */ 'C',
/* 0x2f */ 'V',
/* 0x30 */ 'B',
/* 0x31 */ 'N',
/* 0x32 */ 'M',
/* 0x33 */ ',',
/* 0x34 */ '.',
/* 0x35 */ '/',
/* 0x36 */ 0x00,
/* 0x37 */ '*',
/* 0x38 */ 0x00,
/* 0x39 */ 0x00,
/* 0x3a */ 0x00,
/* 0x3b */ 0x00,
/* 0x3c */ 0x00,
/* 0x3d */ 0x00,
/* 0x3e */ 0x00,
/* 0x3f */ 0x00,
/* 0x40 */ 0x00,
/* 0x41 */ 0x00,
/* 0x42 */ 0x00,
/* 0x43 */ 0x00,
/* 0x44 */ 0x00,
/* 0x45 */ 0x00,
/* 0x46 */ 0x00,
/* 0x47 */ 0x00,
/* 0x48 */ 0x00,
/* 0x49 */ 0x00,
/* 0x4a */ '-',
/* 0x4b */ 0x00,
/* 0x4c */ 0x00,
/* 0x4d */ 0x00,
/* 0x4e */ '+',
/* 0x4f */ 0x00,
/* 0x50 */ 0x00,
/* 0x51 */ 0x00,
/* 0x52 */ 0x00,
/* 0x53 */ 0x00,
/* 0x54 */ 0x00,
/* 0x55 */ 0x00,
/* 0x56 */ 0x00,
/* 0x57 */ 0x00,
/* 0x58 */ 0x00,
/* 0x59 */ 0x00,
/* 0x5a */ 0x00,
/* 0x5b */ 0x00,
/* 0x5c */ 0x00,
/* 0x5d */ 0x00,
/* 0x5e */ 0x00,
/* 0x5f */ 0x00,
/* 0x60 */ 0x00,
/* 0x61 */ 0x00,
/* 0x62 */ 0x00,
/* 0x63 */ 0x00,
/* 0x64 */ 0x00,
/* 0x65 */ 0x00,
/* 0x66 */ 0x00,
/* 0x67 */ 0x00,
/* 0x68 */ 0x00,
/* 0x69 */ 0x00,
/* 0x6a */ 0x00,
/* 0x6b */ 0x00,
/* 0x6c */ 0x00,
/* 0x6d */ 0x00,
/* 0x6e */ 0x00,
/* 0x6f */ 0x00,
/* 0x70 */ 0x00,
/* 0x71 */ 0x00,
/* 0x72 */ 0x00,
/* 0x73 */ 0x00,
/* 0x74 */ 0x00,
/* 0x75 */ 0x00,
/* 0x76 */ 0x00,
/* 0x77 */ 0x00,
/* 0x78 */ 0x00,
/* 0x79 */ 0x00,
/* 0x7a */ 0x00,
/* 0x7b */ 0x00,
/* 0x7c */ 0x00,
/* 0x7d */ 0x00,
/* 0x7e */ 0x00,
/* 0x7f */ 0x00,
};

void init_keybord(){
    kb_stat.scancode = SCANCODE_VERSION;
    info("init keyboard\n");
}

void keyboard_handle(unsigned long nr){
    char key = io_in8(KB_DATA_PORT);
    queue_put(&keyboard_buf, key);
    io_out8(0x20, 0x20);
}

void __kb_analysis_keycode(){
#if SCANCODE_VERSION == 0x01
    unsigned char key = queue_get(&keyboard_buf);
    if(key == 0x00)return;

    unsigned char scancode = scancode_map[key & BREAK_MASK];

    if(key == PRIFIX_CODE){
        kb_wait();
        key = queue_get(&keyboard_buf);
        switch(key & BREAK_MASK){
            case 0x5b:
                  info("INPUT L GUI\n");
                  break;
            case 0x1d:
                  info("INPUT R CTRL\n");
                  break;
            case 0x5c:
                  info("INPUT R GUI\n");
                  break;
            case 0x38:
                  info("INPUT R ALT\n");
                  break;
            case 0x5d:
                  info("INPUT APPS\n");
                  break;
            case 0x2a:
                  info("INPUT PRNT\n");
                  break;
            case 0x37:
                  info("INPUT SCRN\n");
                  break;
            case 0x52:
                  info("INPUT INSERT\n");
                  break;
            case 0x47:
                  info("INPUT HOME\n");
                  break;
            case 0x49:
                  info("INPUT PG UP\n");
                  break;
            case 0x53:
                  info("INPUT DELETE\n");
                  break;
            case 0x4f:
                  info("INPUT END\n");
                  break;
            case 0x51:
                  info("INPUT PG DN\n");
                  break;
            case 0x48:
                  info("INPUT U ARROW\n");
                  break;
            case 0x4b:
                  info("INPUT L ARROW\n");
                  break;
            case 0x50:
                  info("INPUT D ARROW\n");
                  break;
            case 0x4d:
                  info("INPUT A ARROW\n");
                  break;
        }
    }else if(key == 0xe1){
        kb_wait();
        key = queue_get(&keyboard_buf);
        if((key & BREAK_MASK) != 0x1d)return;
        kb_wait();
        key = queue_get(&keyboard_buf);
        if((key & BREAK_MASK) == 0x45)
            info("INPUT PAUSE\n");
    }else{
        switch (key & BREAK_MASK) {
            case 0x2a:
                info("INPUT L SHIFT\n");
                if(key & BREAK_FLAG) LOCK_CAPS_UPDATE();
                else LOCK_CAPS_UPDATE();
                break;
            case 0x36:
                info("INPUT R SHIFT\n");
                if(key & BREAK_FLAG) LOCK_CAPS_UPDATE();
                else LOCK_CAPS_UPDATE();
                break;
            case 0x3a:
                info("INPUT CAPS LOCK\n");
                if(key & BREAK_FLAG)LOCK_CAPS_UPDATE();
                break;
            case 0x45:
                info("INPUT NUM LOCK\n");
                if(key & BREAK_FLAG)LOCK_NUM_UPDATE();
                break;
            case 0x3b:
                info("INPUT F1\n");
                break;
            case 0x3c:
                info("INPUT F2\n");
                break;
            case 0x3d:
                info("INPUT F3\n");
                break;
            case 0x3e:
                info("INPUT F4\n");
                break;
            case 0x3f:
                info("INPUT F5\n");
                break;
            case 0x40:
                info("INPUT F6\n");
                break;
            case 0x41:
                info("INPUT F7\n");
                break;
            case 0x42:
                info("INPUT F8\n");
                break;
            case 0x43:
                info("INPUT F9\n");
                break;
            case 0x44:
                info("INPUT F10\n");
                break;
            case 0x57:
                info("INPUT F11\n");
                break;
            case 0x58:
                info("INPUT F12\n");
                break;
            default:
                if(key & BREAK_FLAG){//BREAK CODE
                    info("KEY=%#04x REPLASE KEY %c\n",key,scancode);
                }else{//MAKE CODE
                    info("KEY=%#04x PRESS KEY %c\n",key,scancode);
                }
        }
    }
#elif SCANCODE_VERSION == 0x02
#error "This SCANCODE_VERSION is not supported"
#elif SCANCODE_VERSION == 0x03
#error "This SCANCODE_VERSION is not supported"
#else
#error "SCANCODE_VERSION range is [1,2,3]"
#endif
}
