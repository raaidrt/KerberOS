#ifndef _SERIAL_H
#define _SERIAL_H 1

#include <io.h>

/* The I/O ports */

/* All the I/O ports are calculated relative to the data port. This is because
    * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
    * order, but they start at different values.
    */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor);

/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com);

/** serial_configure_buffer: 
 *  Configures the buffer of the given serial port. The buffer is set to 
 *  have 14 bytes stored in the FIFO queue, 
 * 
 *  @param com The serial port to configure
*/
void serial_configure_buffer(unsigned short com);

/** serial_initialize - initializes communication with the serial port */
void serial_initialize();

/** is_transmit_empty - is a function that returns 0 if 
 *  the buffer is empty, and returns a non-zero integer if 
 *  the buffer is non-empty
 */
int is_transmit_empty();

/** serial_putchar - writes a single character to the serial port
 * 
 *  @param c The character to write
 */
void serial_putchar(unsigned char c);

#endif