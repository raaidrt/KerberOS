    #ifndef INCLUDE_IO_H
    #define INCLUDE_IO_H 1

    /** 
     * outb - Sends the given data to the given I/O port. Defined in io.s
     *
     * @port: The I/O port to send the data to
     * @data: The data to send to the I/O port
     */
    void outb(unsigned short port, unsigned char data);

    #endif // INCLUDE_IO_H