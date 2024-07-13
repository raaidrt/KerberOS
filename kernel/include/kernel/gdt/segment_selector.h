#ifndef _SEGMENT_SELECTOR_H
#define _SEGMENT_SELECTOR_H

#include <stdint.h>

/** 
 * Segment Selector
 * - (RPL) Requested Privilege Level
 * - (TI) 0 = GDT, 1 = LDT
 * - (index) The Index in the GDT or LDT to use
 */
struct segment_selector {
    uint16_t rpl: 2;
    uint16_t ti: 1;
    uint16_t index: 13;
};

/** 
 * Encodes Segment Selector
 * @param rpl Requested Privilege Level
 * @param ti specifies which table to use {0 = GDT, 1 = LDT }
 * @param index The Index in the GDT or LDT to use
 */

struct segment_selector encode_segment_selector(
    uint16_t rpl,
    uint16_t ti,
    uint16_t index
);

#endif
