#include "../ports/ports.h"
#include "timer.h"

#define WAIT_FOR(check) \
    while (!(check))    \
    {                   \
    };

uint32 timer_read_counter(void)
{
    outb(PIT_MODE_COMMAND, 0);
    uint32 counter = inb(PIT_DATA_PORT0);
    counter |= inb(PIT_DATA_PORT0) << 8;

    return counter;
}

void timer_init(int freq_divisor)
{
    uint16 divisor = PIT_FREQUENCY / freq_divisor;

    outb(PIT_MODE_COMMAND, PIT_CHANNEL1 | PIT_LOWBYTE | PIT_SQUARE_WAVE);
    outb(PIT_DATA_PORT0, divisor & 0xFF);
    outb(PIT_DATA_PORT0, (divisor >> 8) & 0xFF);
}

void timer_sleep(uint16 ms)
{
    uint16 wait_val = (ms);

    outb(PIT_MODE_COMMAND, PIT_CHANNEL1 | PIT_LOWBYTE);
    outb(PIT_DATA_PORT0, wait_val & 0xFF);
    outb(PIT_DATA_PORT0, (wait_val >> 8) & 0xFF);

    WAIT_FOR(timer_read_counter() == 0);
}