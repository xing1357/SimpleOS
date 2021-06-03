#ifndef PORTS_H
#define PORTS_H

#include "../../types.h"

uint8 inb(uint16 port);
void outb(uint16 port, uint8 data);
uint16 inports(uint16 _port);
void outports(uint16 _port, uint16 _data);
uint32 inportl(uint16 _port);
void outportl(uint16 _port, uint32 _data);
void wait_for_io(uint32 timer_count);
void sleep(uint32 timer_count);

#endif