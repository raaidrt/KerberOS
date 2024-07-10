/** 
 * Global Descriptor Table (GDT) Assembly Commands
 *
 * Author: Raaid Tanveer <raaidraiyantanveer@mgmail.com>
 */

#ifndef _GDT_ASM_H
#define _GDT_ASM_H

/** 
 * Loads the value 0x10 to the segment registers ds, es, fs, gs, and ss, and 
 * the value 0x08 to the code segment register cs.
 */
void load_segment_registers();

/**  
 * Loads the Global Descriptor Table (GDT) into the GDTR register
 *
 * @param gdt is the struct that contains the Global Descriptor Table (GDT)
 */
void load_gdt(struct gdt gdt);

#endif
