#include "keyboard.h"
#include "print.h"
#include "lib.h"

void keyboard_handle(unsigned long nr){
    char key = io_in8(KB_DATA_PORT);
    io_out8(0x20, 0x20);
}
