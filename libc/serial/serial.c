#include <serial.h>

void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),
            SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),
            (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),
            divisor & 0x00FF);
}

void serial_configure_line(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_buffer(unsigned short com)
{
    /*
     * Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
     * Content: | lvl | bs | r | dma | clt | clr | e | 
     * Value:   | 1 1 | 0  | 0 | 0   | 1   | 1   | 1 |
     */
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0b11000111);
}

void serial_intiailize()
{
    serial_configure_baud_rate(SERIAL_COM1_BASE, 2);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_buffer(SERIAL_COM1_BASE);
}

int is_transmit_empty() {
   return inb(SERIAL_LINE_STATUS_PORT(SERIAL_COM1_BASE)) & 0x20;
}
 
void serial_putchar(unsigned char c) {
   while (is_transmit_empty() == 0);
 
   outb(SERIAL_COM1_BASE, c);
}