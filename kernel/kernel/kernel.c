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
	printf("Hello, %s World %d!\nHere is another line\n", "kernel", 42);
	gdt_initialize();
	idt_initialize();
	// set_interrupt_flag();
	printf("GDT Intialization complete\n");
}
