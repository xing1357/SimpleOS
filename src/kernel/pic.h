/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#ifndef PIC_H
#define PIC_H

#include "types.h"
#include "drivers/ports/ports.h"

#define IRQ_NUM 16

#define IRQ_TIMER 0

#define IRQ_KEYBOARD 1

#define IRQ_TO_SLAVE_PIC 2

#define IRQ_COM2_4 3

#define IRQ_COM1_3 4

#define IRQ_LPT2 5

#define IRQ_FLOPPY 6

#define IRQ_LPT1 7

#define IRQ_REAL_TIME_CLOCK 8

#define IRQ_AVAILABLE_1 9

#define IRQ_AVAILABLE_2 10

#define IRQ_AVAILABLE_3 11

#define IRQ_MOUSE 12

#define IRQ_MATH_CPU 13

#define IRQ_FIRST_HD 14

#define IRQ_SECOND_HD 15

void pic_init_irq();

int pic_irq_enable(uint32 irq);

int pic_irq_disable(uint32 irq);

void pic_send_eoi(uint32 irq);

#endif