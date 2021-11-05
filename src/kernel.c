#include "drivers/vga/display.h"
#include "malloc.h"

#include "drivers/kbd/kbd.h"
#include "interrupts/isr.h"

void testfunc(registers_t* r) {
    print_string("Called!\n");
}

void main(void) {
    // asm volatile("sti"); // < problem here
    
    print_string("Hello world");

    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    isr_install();

    print_string("Enabling external interrupts.\n");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_kbd();

    return;
}