#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>
#include <kernel/interrupt/interrupt_descriptor.h>
#include <kernel/descriptor_table.h>

DESCRIPTOR_TABLE(idt, 256)

#define NUM_INTERRUPTS 32

void idt_initialize();

void print_idt_entry(struct idt_entry entry);

#endif
