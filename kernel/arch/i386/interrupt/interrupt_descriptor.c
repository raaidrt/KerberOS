#include <kernel/interrupt/interrupt_descriptor.h>

struct idt_entry encode_idt_entry(
    uint32_t offset, 
    struct segment_selector segment_selector,
    uint8_t reserved,
    enum interrupt_gate_kind gate_type,
    uint8_t dpl,
    uint8_t p
) 
{
    struct idt_entry entry;
    entry.offset_low = offset & 0xffff;
    entry.offset_high = (offset >> 16) & 0xff;
    entry.segment_selector = segment_selector;
    entry.reserved = reserved;
    entry.gate_type = gate_type;
    entry.zero = 0;
    entry.dpl = dpl;
    entry.p = p;
    return entry;
}
