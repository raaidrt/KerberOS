#include <kernel/gdt/gdt.h>
#include <kernel/gdt/gdt_asm.h>
#include <kernel/interrupt/flag.h>
#include <logger.h>

void gdt_initialize() {
    dbg_logf("Disabling Interrupts\n");
    clear_interrupt_flag();
    
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

    gdt_pointer.address = (unsigned int) gdt;
    gdt_pointer.size = sizeof(gdt);

    load_gdt(gdt_pointer);
    dbg_logf("DONE\n");

    dbg_logf("Loading segment registers...");
    load_segment_registers();
    dbg_logf("DONE\n");

}

