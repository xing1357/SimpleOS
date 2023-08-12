include "io_ports.h"

void serial_init()
{
    outportb(
        SERIAL_COM1_INTERRUPT_ENABLE_PORT,
        0x00); // Disable all interrupts
    outportb(
        SERIAL_COM1_LINE_CTRL_REG_PORT,
        0x80); // Enable DLAB (set baud rate divisor)
    outportb(
        SERIAL_COM1_INTERRUPT_DIVISOR_LOW,
        0x03); // Set divisor to 3 (lo byte) 38400 baud
    outportb(
        SERIAL_COM1_INTERRUPT_DIVISOR_HIGH,
        0x00); //                  (hi byte)
    outportb(
        SERIAL_COM1_LINE_CTRL_REG_PORT,
        0x03); // 8 bits, no parity, one stop bit
    outportb(
        SERIAL_COM1_INT_ID_FIFO_CTRL_PORT,
        0xC7); // Enable FIFO, clear them, with 14-byte threshold
    out_byte(
        SERIAL_COM1_MODEM_CTRL_REG_PORT,
        0x0B); // IRQs enabled, RTS/DSR set
}
/**
 * @brief Is available to read?
 * 
 * @return int 
 */
static inline int serial_received()
{
    return inportb(SERIAL_COM1_LINE_STATUS_REG_PORT) & 1;
}

/**
 * @brief Is the transit empty?
 * 
 * @return int 
 */
static inline int is_transmit_empty()
{
    return inportb(SERIAL_COM1_LINE_STATUS_REG_PORT) & 0x20;
}

/**
 * @brief Get a character from the serial port
 * 
 * @return char 
 */
char serial_getc()
{
    while (serial_received() == 0)
        ;

    return (char)inportb(SERIAL_COM1_DATA_REGISTER_PORT);
}

/**
 * @brief Write a character to the serial port
 * 
 * @param c 
 */
void serial_putc(char c)
{
    while (is_transmit_empty() == 0)
        ;

    outportb(SERIAL_COM1_DATA_REGISTER_PORT, c);
}

/**
 * @brief Prints a string the serial output
 * 
 * @param string 
 */
void serial_puts(char *string)
{
    int i = 0;
    while (string[i])
    {
        serial_putc(string[i]);
        i++;
    }
}
