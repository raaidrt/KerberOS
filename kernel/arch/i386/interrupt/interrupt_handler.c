#include <kernel/interrupt/interrupt_handler.h>
#include <logger.h>

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
    (void) interrupt;
}
