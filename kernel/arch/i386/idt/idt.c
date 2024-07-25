#include <kernel/idt/idt.h>
#include <kernel/interrupt/interrupt_descriptor.h>
#include <kernel/gdt/segment_selector.h>
#include <kernel/idt/idt_asm.h>
#include <kernel/irq/irq.h>

extern int32_t interrupt_handler_table[NUM_INTERRUPTS];

void idt_initialize() {
    for (int i = 0; i < NUM_INTERRUPTS; i++) {
	idt[i] = encode_idt_entry(
	    interrupt_handler_table[i],
	    encode_segment_selector(0, 0, 0x08),
	    0,
	    TRAP_GATE_32_BITS,
	    0, 
	    1
	);
    }

    idt_pointer.base = (uint32_t) &idt;
    idt_pointer.limit = NUM_INTERRUPTS * sizeof(struct idt_entry) - 1;
    load_idt(idt_pointer);
}
