#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <logger.h>
#include <serial.h>

#define __is_myos_kernel 1

void kernel_main(void) {
	terminal_initialize();
	gdt_initialize();
	dbg_logf("Hey there, this is a logged statement %d\n", 1);
	printf("Hello, %s World %d!\nHere is another line\n", "kernel", 42);
}
