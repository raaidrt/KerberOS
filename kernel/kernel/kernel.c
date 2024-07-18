#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt/gdt.h>
#include <logger.h>
#include <serial.h>
#include <kernel/idt/idt.h>
#include <kernel/interrupt/flag.h>

#define __is_kerberos_kernel 1

void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel word %x!\n", 0x42);
	printf("Initializing the Global Descriptor Table\n");
	gdt_initialize();
	printf("Initializing the Interrupt Descriptor Table\n");
	idt_initialize();
	printf("Setting the Interrupt Flag\n");
	set_interrupt_flag();
	printf("DONE\n");
	printf("Start shell\n");
}
