#ifndef _INTERRUPT_DESCRIPTOR_H
#define _INTERRUPT_DESCRIPTOR_H
/** 
 * Interrupt Descriptor Table (IDT) Entry
 *
 * Author: Raaid Tanveer <raaidraiyantanveer@gmail.com>
 */

#include <stdint.h>
#include <kernel/gdt/segment_selector.h>

enum interrupt_gate_kind {
    TASK_GATE = 0b0101,
    INTERRUPT_GATE_16_BITS = 0b0110,
    INTERRUPT_GATE_32_BITS = 0b1110,
    TRAP_GATE_16_BITS = 0b0111,
    TRAP_GATE_32_BITS = 0b1111
};

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
    struct segment_selector segment_selector;
    uint8_t reserved; 
    enum interrupt_gate_kind gate_type: 4;
    uint8_t zero: 1;
    uint8_t dpl: 2; 
    uint8_t p: 1; 
    uint16_t offset_high; 
}__attribute__((packed));

/** 
 * Encodes an Interrupt Descriptor Table (IDT) Entry 
 *
 * @param offset The 32-bit address in the segment
 * @param segment_selector The offset in the GDT
 * @param reserved
 * @param gate_type
 * @param dpl Descriptor Privilege Level
 * @param p Present bit
 */ 
struct idt_entry encode_idt_entry(
    uint32_t offset, 
    struct segment_selector segment_selector,
    uint8_t reserved,
    enum interrupt_gate_kind gate_type,
    uint8_t dpl,
    uint8_t p
);

#endif
