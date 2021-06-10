#ifndef PIC_H
#define PIC_H

#include "types.h"
#include "drivers/ports/ports.h"

void pic();
void pic_acknowledge(uint8 irq);

#endif