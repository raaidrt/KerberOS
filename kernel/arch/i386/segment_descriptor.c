#include <kernel/segment_descriptor.h>

struct access encode_access_byte(
    uint8_t a, 
    uint8_t rw, 
    uint8_t dc, 
    uint8_t e, 
    uint8_t s, 
    uint8_t dpl, 
    uint8_t p) 
{
    struct access access;
    access.a = a;
    access.rw = rw;
    access.dc = dc;
    access.e = e;
    access.s = s;
    access.dpl = dpl;
    access.p = p;

    return access;
}

struct gdt_entry encode_gdt_entry(
    uint32_t base,
    uint32_t limit, 
    struct access access,
    uint8_t flags
) 
{
    uint16_t base_low = base & 0xffff;
    uint8_t base_mid = (base >> 16) & 0xff;
    uint8_t base_high = (base >> 24) & 0xff;

    uint16_t limit_low = limit & 0xffff;
    uint8_t limit_high = (limit >> 16) & 0xf;

    struct gdt_entry entry;
    entry.base_low = base_low;
    entry.base_mid = base_mid;
    entry.base_high = base_high;

    entry.limit_low = limit_low;
    entry.limit_high = limit_high;

    entry.access = access;
    entry.flags = flags;

    return entry;
}
