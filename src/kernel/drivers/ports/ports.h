#ifndef PORTS_H
#define PORTS_H

#include "../../types.h"

uint8 inb(uint16 port);
void outb(uint16 port, uint8 data);
void wait_for_io(uint32 timer_count);
void sleep(uint32 timer_count);

#endif