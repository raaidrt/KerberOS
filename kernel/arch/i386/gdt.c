#include <kernel/gdt.h>
#include <kernel/gdt_asm.h>
#include <logger.h>

void gdt_initialize() {
    dbg_logf("Disabling Interrupts\n");
    disable_interrupts();
    
    dbg_logf("Initializing the Global Descriptor Table...");
    struct access null_descriptor_access;
    gdt[0] = encode_gdt_entry(0, 0, null_descriptor_access, 0);
    
    // Code segment
    gdt[1] = encode_gdt_entry(
        0,
        0xFFFFF,
        encode_access_byte(0, 1, 0, 1, 1, 0, 1),
        0xC
    );
    
    // Data segment
    gdt[2] = encode_gdt_entry(
        0,
        0xFFFFF,
        encode_access_byte(0, 1, 0, 0, 1, 0, 1),
        0xC
    );

    gp.address = (unsigned int) gdt;
    gp.size = sizeof(gdt);

    load_gdt(gp);
    dbg_logf("DONE\n");

    dbg_logf("Loading segment registers...");
    load_segment_registers();
    dbg_logf("DONE\n");

}

/* 
uint16_t encode_gdt_entry(uint16_t offset, uint8_t ti, uint8_t rpl)
{ */
    /*
     * Bit:     | 15                                3 | 2  | 1 0 |
     * Content: | offset (index)                      | ti | rpl |
     */ /*
    return offset << 3 
         | ti << 2
         | rpl; 
}
*/
