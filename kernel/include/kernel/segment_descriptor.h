#ifndef _SEGMENT_DESCRIPTOR_H
#define _SEGMENT_DESCRIPTOR_H

#include <stdint.h>

/** 
  * Access byte (Documentation from <osdev.org>)
  * - (P) Present bit: Allows an entry to refer to a valid segment. 
  *     Must be set to 1 for any valid segment.
  * - (DPL) Descriptor Privilege Level: Contains the CPU Privilege level 
  *     of the segment. 
  *     0 = highest privilege (kernel), 3 = lowest privilege 
  *     (user applications).
  * - (S) Descriptor Type Bit: If clear (0) the descriptor defines a 
  *     system segment (eg. a Task State Segment). 
  *     If set (1) it defines a code or data segment.
  * - (E) Executable Bit: If clear (0) the descriptor defines a data segment. 
  *     If set (1) it defines a code segment which can be executed from.
  * - (DC) Direction Bit / Conforming Bit: 
  *   - For data selectors: Direction bit. 
  *         If clear (0) the segment grows up. 
  *         If set (1) the segment grows down, 
  *         ie. the Offset has to be greater than the Limit.
  *   - For code selectors: Conforming bit.
  *         If clear (0) code in this segment can only be executed 
  *         from the ring set in DPL.
  *         If set (1) code in this segment can be executed from an 
  *         equal or lower privilege level.
  * - (RW) Readable Bit/Writable Bit:
  *     For code segments: Readable bit. 
  *         If clear (0), read access for this segment is not allowed. 
  *         If set (1) read access is allowed. 
  *         Write access is never allowed for code segments.
  * - (A) Accessed Bit:
  *     The CPU will set it when the segment is accessed unless set 
  *     to 1 in advance. This means that in case the GDT descriptor is stored 
  *     in read only pages and this bit is set to 0, the CPU trying to set 
  *     this bit will trigger a page fault. Best left set to 1 unless 
  *     otherwise needed.
  */
struct access {
    uint8_t a: 1;
    uint8_t rw: 1;
    uint8_t dc: 1;
    uint8_t e: 1;
    uint8_t s: 1;
    uint8_t dpl: 2;
    uint8_t p: 1;
}__attribute__((packed));

struct access encode_access_byte(
    uint8_t a, 
    uint8_t rw, 
    uint8_t dc, 
    uint8_t e, 
    uint8_t s, 
    uint8_t dpl, 
    uint8_t p
);

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    struct access access;
    uint8_t limit_high: 4;
    uint8_t flags: 4;
    uint8_t base_high;
}__attribute__((packed));

struct gdt_entry encode_gdt_entry(
    uint32_t base, 
    uint32_t limit, 
    struct access access, 
    uint8_t flags
);

#endif
