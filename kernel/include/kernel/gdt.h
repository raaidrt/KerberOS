#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

struct gdt {
    unsigned int address;
    unsigned short size;
} __attribute__((packed));

uint16_t gdt_entries[3]; 
struct gdt gp; // this is the GDT pointer that will be loaded by LGDT

void gdt_initialize();
void load_gdt(struct gdt *gdt);
uint16_t encode_gdt_entry(uint16_t offset, uint8_t ti, uint8_t rpl);

#endif
