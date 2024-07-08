#ifndef _GDT_ASM_H
#define _GDT_ASM_H



void disable_interrupts();
void load_segment_registers();
void load_gdt(struct gdt gdt);

#endif
