#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>
#include <kernel/interrupt/interrupt_descriptor.h>
#include <kernel/descriptor_table.h>

DESCRIPTOR_TABLE(idt, 256)

void idt_initialize();

#endif
