#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>
#include <kernel/segment_descriptor.h>

struct gdt_entry gdt[3]; 

struct gdt {
    unsigned short size;
    unsigned int address;
} __attribute__((packed));

struct gdt gp;

void gdt_initialize();

#endif
