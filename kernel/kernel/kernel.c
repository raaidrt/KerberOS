#include <stdio.h>

#include <kernel/tty.h>

#define __is_myos_kernel 1

void kernel_main(void) {
	terminal_initialize();
	printf("Hello, %s World %d!\nHere is another line\n", "kernel", 42);
}
