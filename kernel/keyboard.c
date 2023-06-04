#include "keyboard.h"
#include "print.h"
#include "lib.h"

array_queue keyboard_buf = INIT_ARRAY_QUEUE(128);

void keyboard_handle(unsigned long nr){
    char key = io_in8(KB_DATA_PORT);
    queue_put(&keyboard_buf, key);
    io_out8(0x20, 0x20);
}
