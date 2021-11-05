#include "kbd.h"
#include "../ports/ports.h"
#include "../vga/display.h"
#include <stdint.h>

#include "../../interrupts/isr.h"
#include "../../utils/util.h"

static void keyboard_callback(registers_t* r) {
    uint8_t scancode = port_byte_in(0x60);
    char p[0xff];
    int_to_string(scancode, p);
    print_string(p);
}

void init_kbd() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}