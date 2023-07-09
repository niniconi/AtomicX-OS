#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "lib.h"

#define KB_DATA_PORT 0x60
#define KB_CTRL_PORT 0x64

/*
 * send control command to 0x64(KB_CTRL_PORT),send param to 0x60(KB_DATA_PORT),return from 0x60(KB_DATA_PORT)
 */
#define IKB_CMD_RDKEY 0x20
#define IKB_CMD_CONF 0x60
#define IKB_CMD_MOUSE_FORBID 0xa7
#define IKB_CMD_MOUSE_ALLOW 0xa8
#define IKB_CMD_SYSREBOOT 0xef

/*
 * send conrtrol command to 0x60(KB_DATA_PORT),send param to 0x60(KB_DATA_PORT),return from 0xfa
 */
#define KB_CMD_REBOOT 0xff
#define KB_CMD_RESEND 0xfe
#define KB_CMD_DEFAULTSPEED 0xf6
#define KB_CMD_CLOSESCAN 0xf5
#define KB_CMD_OPENSCAN 0xf4
#define KB_CMD_SCANCODE 0xf0

#define BREAK_FLAG 0x80
#define BREAK_MASK 0x7f

#define NR_SCANCODE 128
#define PRIFIX_CODE 0xe0

#define INIT_KB_STAT() { \
    0, \
    0x01 \
}

#define LOCK_CAPS_UPDATE() (kb_stat.flags = (kb_stat.flags & 0x01) ? kb_stat.flags & ~0x01 : kb_stat.flags & 0x01)
#define LOCK_NUM_UPDATE() ( kb_stat.flags = (kb_stat.flags & 0x02) ? kb_stat.flags & ~0x02 : kb_stat.flags & 0x02)

#define kb_wait() while(keyboard_buf.amount==0)

typedef unsigned char scancode_t;

struct keyboard_stat{
    unsigned long flags;
    scancode_t scancode;
};

void keyboard_handle(unsigned long nr);
void init_keybord();
void kb_analysis_keycode();

def_forbid_intr_function(kb_analysis_keycode);

#endif
