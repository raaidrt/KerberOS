#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt/gdt.h>
#include <logger.h>
#include <serial.h>
#include <kernel/idt/idt.h>
#include <kernel/interrupt/flag.h>
#include <kernel/irq/irq.h>
#include <kernel/time/time.h>

#define __is_kerberos_kernel 1

void kernel_main(void) {
	dbg_logf(INCLUDE_TIME, INFO, "Initializing the terminal...");
	terminal_initialize();
	dbg_logf(EXCLUDE_TIME, NONE, "DONE\n");
	printf("Hello, kernel word %x!\n", 0x42);
	dbg_logf(INCLUDE_TIME, INFO, "Initializing the Global Descriptor Table...");
	gdt_initialize();
	dbg_logf(EXCLUDE_TIME, NONE, "DONE\n");
	dbg_logf(INCLUDE_TIME, INFO, "Initializing the Interrupt Descriptor Table...");
	idt_initialize();
	dbg_logf(EXCLUDE_TIME, NONE, "DONE\n");
	dbg_logf(INCLUDE_TIME, INFO, "Remapping the IRQ lines...");
	pic_remap(0x20, 0x2F);
	dbg_logf(EXCLUDE_TIME, NONE, "DONE\n");
	set_interrupt_flag();
}
