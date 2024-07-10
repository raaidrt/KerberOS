/** 
 * Interrupt Descriptor Table (IDT) Entry
 *
 * Author: Raaid Tanveer <raaidraiyantanveer@gmail.com>
 */

#include <stdint.h>

/**
 * An entry in the Interrupt Descriptor Table (IDT)
 * - (offset_high) and (offset_low) make up the 32-bit address in the segment
 * - (p) If the handler is present in memory or not 
 *      (1 = present, 0 = not present).
 * - (DPL) Descriptor Privilege Level: the privilege level in the 
 *      handler can be called from {0, 1, 2, 3}. 
 * - (D) Size of gate: {1 = 32 bits, 0 = 16 bits}
 * - (segment selector) The offset in the GDT
 * - (R) reserved
 */ 
struct idt_entry {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved: 5;
    uint8_t zero_low: 3;
    uint8_t one_one_zero: 3;
    uint8_t d: 1;
    uint8_t zero_high: 1;
    uint8_t dpl: 2;
    uint8_t p: 1;
    uint8_t offset_high: 8;
}__attribute__((packed));

/** 
 * Encodes an Interrupt Descriptor Table (IDT) Entry 
 *
 * @param offset The 32-bit address in the segment
 * @param segment_selector The offset in the GDT
 * @param reserved 
 * @param d size of gate {1 = 32 bits, 0 = 16 bits}
 * @param dpl Descriptor Privilege Level
 * @param p Present bit
 */ 
struct idt_entry encode_idt_entry(
    uint32_t offset, 
    uint16_t segment_selector,
    uint8_t reserved,
    uint8_t d,
    uint8_t dpl,
    uint8_t p
);
