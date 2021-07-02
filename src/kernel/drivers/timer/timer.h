#pragma once

#define PIT_FREQUENCY 11931820
#define PIT_USEC 1000

extern uint32 timer_read_counter(void);

enum pit_reg
{
    PIT_MODE_COMMAND = 0x43,
    PIT_DATA_PORT0 = 0x40,
};

enum pit_usage
{
    PIT_CHANNEL1 = 1 << 5,
    PIT_LOWBYTE = 1 << 4,
    PIT_SQUARE_WAVE = 6
};

void timer_initialize(int frequency);

void timer_sleep(uint16 ms);