#include <stdio.h>

#include <kernel/tty.h>
#include <serial.h>

#define __is_myos_kernel 1

void kernel_main(void) {
	terminal_initialize();
	serial_initialize();
	serial_putchar('c');
	printf("Hello, %s World %d!\nHere is another line\n", "kernel", 42);
}
