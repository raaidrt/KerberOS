#include <kernel/gdt/segment_selector.h>

struct segment_selector encode_segment_selector(
    uint16_t rpl, 
    uint16_t ti, 
    uint16_t index
) 
{
    struct segment_selector segment_selector;
    segment_selector.rpl = rpl;
    segment_selector.ti = ti;
    segment_selector.index = index >> 3;
    return segment_selector;
}
