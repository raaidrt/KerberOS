#include <stdio.h>

#include <kernel/tty.h>
#include <logger.h>

#define __is_myos_kernel 1

void kernel_main(void) {
	terminal_initialize();
	serial_initialize();
	dbg_logf("Hey there, this is a logged statement %d\n", 1);
	printf("Hello, %s World %d!\nHere is another line\n", "kernel", 42);
}
