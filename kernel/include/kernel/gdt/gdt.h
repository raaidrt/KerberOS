/** 
 * Global Descriptor Table (GDT) Initialization
 *
 * Author: Raaid Tanveer <raaidraiyantanveer@gmail.com>
 */

#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>
#include <kernel/gdt/segment_descriptor.h>
#include <kernel/descriptor_table.h>

DESCRIPTOR_TABLE(gdt, 3)

/** 
 * Initializes the GDT with three segment descriptors
 * 1. The NULL descriptor
 * 2. The Code Segment at offset 0x08
 * 3. The Data Segment at offset 0x10 
 * 
 * Both the code segemnt and the data segment have a base address
 * of 0x00000, and a limit of 0xFFFFF. 
 * 
 * The function will clear the interrupts flag.
 */
void gdt_initialize();

#endif
