#include <kernel/gdt.h>
#include <logger.h>

void gdt_initialize() {
    // set up the fields in the Global Descriptor Table
    gdt_entries[0] = encode_gdt_entry(0, 0, 0);
    gdt_entries[1] = encode_gdt_entry(0x08, 0b1010, 0);
    gdt_entries[2] = encode_gdt_entry(0x10, 0b0010, 0);

    gp.address = (unsigned int) (unsigned long) gdt_entries;
    gp.size = sizeof(gp);

    load_gdt(gp);

    dbg_logf("The Global Descriptor Table has been initialized\n");
}

void load_gdt(struct gdt gdt)
{    
    __asm__ __volatile__ (
        "lgdt %0\n\t"
        :
        : "m" (gdt)
        : "memory"
    );
}

uint16_t encode_gdt_entry(uint16_t offset, uint8_t ti, uint8_t rpl)
{
    /*
     * Bit:     | 15                                3 | 2  | 1 0 |
     * Content: | offset (index)                      | ti | rpl |
     */ 
    return offset << 3 
         | ti << 2
         | rpl; 
}

// TODO: Write section for loading code segment into the segment registers
