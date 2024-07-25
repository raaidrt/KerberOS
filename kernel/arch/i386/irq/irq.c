#include <kernel/irq/irq.h>
#include <io.h>

#define IRQ_MASTER_OFFSET 0x20
#define IRQ_SLAVE_OFFSET 0x2F

static inline void io_wait() {
    outb(0x80, 0);
}

void pic_send_eoi(uint8_t irq) 
{
    if (irq >= 8) {
        // slave
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_remap(uint8_t master_base_offset, uint8_t slave_base_offset) 
{
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, master_base_offset);    // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, slave_base_offset);     // ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 4);                     // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);                     // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait(); 
    
    outb(PIC1_DATA, ICW4_8086);             // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
    
    outb(PIC1_DATA, 0);                     // reset data ports
    io_wait();
    outb(PIC2_DATA, 0);
}

void irq_set_mask(uint8_t irq) 
{
    uint16_t port;
    uint8_t value;

    if(irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);        
}

void irq_clear_mask(uint8_t irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | ~(1 << irq);
    outb(port, value);
}

