#include "pic.h"

#define PIC_MASTER_CTRL 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CTRL  0xA0
#define PIC_SLAVE_DATA  0xA1


void pic() {

    // ICW1
    outb(PIC_MASTER_CTRL, 0x11);  // init master PIC
    outb(PIC_SLAVE_CTRL, 0x11);   // init slave PIC
    // ICW2
    outb(PIC_MASTER_DATA, 0x20);  // IRQ 0..7 remaped to 0x20..0x27
    outb(PIC_SLAVE_DATA, 0x28);   // IRQ 8..15 remaped to 0x28..0x37
    // ICW3
    outb(PIC_MASTER_DATA, 0x04);  // set as Master
    outb(PIC_SLAVE_DATA, 0x02);   // set as Slave
    // ICW4
    outb(PIC_MASTER_DATA, 0x01);  // set x86 mode
    outb(PIC_SLAVE_DATA, 0x01);   // set x86 mode

    outb(PIC_MASTER_DATA, 0xFF);  // all interrupts disabled
    outb(PIC_SLAVE_DATA, 0xFF);

    __asm__ __volatile__("nop");
}

void pic_acknowledge(uint8 irq)
{
  if ( irq > 7)
    outb(PIC_SLAVE_CTRL, 0x20);

  outb(PIC_MASTER_CTRL, 0x20);
}