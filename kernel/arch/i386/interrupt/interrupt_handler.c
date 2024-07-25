#include <kernel/interrupt/interrupt_handler.h>
#include <kernel/irq/irq.h>
#include <logger.h>
#include <panic.h>
#include <io.h>
#include <stdbool.h>

#define KBD_DATA_PORT   0x60

unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}

void interrupt_handler(
    struct cpu_state cpu, 
    struct stack_state stack, 
    uint32_t interrupt
) 
{
    // TODO:  Based on the interrupt number and the CPU state, take 
    // the respective action
    (void) cpu;
    (void) stack;
    
    bool send_eoi = false;
    if (32 <= interrupt && interrupt < 32 + 16) {
	// handling an IRQ
	send_eoi = true;
    }

    if (interrupt == 8 
	|| (10 <= interrupt && interrupt <= 14) 
	|| interrupt == 17 || interrupt == 30) {
	panic("An exception interrupt has been encountered\n");
    }

    if (send_eoi) pic_send_eoi(interrupt - 32);
}
