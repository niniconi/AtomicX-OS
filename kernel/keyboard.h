#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KB_DATA_PORT 0x60
#define KB_CTRL_PORT 0x64

#define KB_CMD_RDKEY 0x20
#define KB_CMD_CONF 0x60
#define KB_CMD_MOUSE_FORBID 0xa7
#define KB_CMD_MOUSE_ALLOW 0xa8
#define KB_CMD_REBOOT 0xef

void keyboard_handle(unsigned long nr);

#endif
