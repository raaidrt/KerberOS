#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt/gdt.h>
#include <logger.h>
#include <serial.h>
#include <kernel/idt/idt.h>
#include <kernel/interrupt/flag.h>
#include <kernel/irq/irq.h>
#include <kernel/time/time.h>
#include <kernel/multiboot/multiboot.h>

#define __is_kerberos_kernel 1

extern char _kernel_virtual_start, _kernel_virtual_end;

void kernel_main(uintptr_t ebx) {
	multiboot_info_t *mbinfo = (multiboot_info_t *) (ebx + 0xC0000000);
	uint32_t mb_mem_lower = mbinfo->mem_lower;
	uint32_t mb_mem_upper = mbinfo->mem_upper;
	dbg_logf(INCLUDE_TIME, INFO, "Multiboot mem_lower = %d, mem_upper = %d\n", mb_mem_lower, mb_mem_upper);
	uintptr_t kstart = (uintptr_t) &_kernel_virtual_start;
	uintptr_t kend = (uintptr_t) &_kernel_virtual_end;
	int ksize = (int) kend - (int) kstart;
	dbg_logf(INCLUDE_TIME, INFO, "kernel start = %x, kernel end = %x, kernel size = %d\n", kstart, kend, ksize);
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
