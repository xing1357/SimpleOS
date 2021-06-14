#include "pic.h"

#define EOI 0x20

#define MASTER_PORT_COMMAND 0x20

#define MASTER_PORT_DATA (MASTER_PORT_COMMAND + 1)

#define SLAVE_PORT_COMMAND 0xA0

#define SLAVE_PORT_DATA (SLAVE_PORT_COMMAND + 1)

#define ICW1_ICW4 0x01

#define ICW1_SINGLE 0x02

#define ICW1_INTERVAL4 0x04

#define ICW1_LEVEL 0x08

#define ICW1_INIT 0x10

#define ICW4_8086 0x01

#define ICW4_AUTO 0x02

#define ICW4_BUF_SLAVE 0x08

#define ICW4_BUF_MASTER 0x0C

#define ICW4_SFNM 0x10

#define PIC_READ_IRR 0x0A

#define PIC_READ_ISR 0x0B

static uint8 master_cur_mask;

static uint8 slave_cur_mask;

void pic_init_irq()
{
	master_cur_mask = 0xFF;
	slave_cur_mask = 0xFF;

	outb(MASTER_PORT_COMMAND, ICW1_INIT + ICW1_ICW4);
	outb(SLAVE_PORT_COMMAND, ICW1_INIT + ICW1_ICW4);

	outb(MASTER_PORT_DATA, 0x20);
	outb(SLAVE_PORT_DATA, 0x28);

	outb(MASTER_PORT_DATA, 4);

	outb(SLAVE_PORT_DATA, 2);

	outb(MASTER_PORT_DATA, ICW4_8086);
	outb(SLAVE_PORT_DATA, ICW4_8086);

	outb(MASTER_PORT_DATA, master_cur_mask);
	outb(SLAVE_PORT_DATA, slave_cur_mask);

	pic_irq_enable(IRQ_TO_SLAVE_PIC);

	outb(0xFF, MASTER_PORT_DATA);
	outb(0xFF, SLAVE_PORT_DATA);
}

int pic_irq_enable(uint32 irq)
{
	if (irq >= IRQ_NUM) {
		return -1;
	}

	uint8 cur_mask;
	uint8 new_mask;

	if (irq < 8) {
		new_mask = ~(1 << irq);
		cur_mask = inb(MASTER_PORT_DATA);
		outb(MASTER_PORT_DATA, (new_mask & cur_mask));
		master_cur_mask = (new_mask & cur_mask);
	} else {
		irq -= 8;
		new_mask = ~(1 << irq);
		cur_mask = inb(SLAVE_PORT_DATA);
		outb(SLAVE_PORT_DATA, (new_mask & cur_mask));
		slave_cur_mask = (new_mask & cur_mask);
	}

	return 0;
}

int pic_irq_disable(uint32 irq)
{
	if (irq >= IRQ_NUM) {
		return -1;
	}

	uint8 cur_mask;
	if (irq < 8) {
		cur_mask = inb(MASTER_PORT_DATA);
		cur_mask |= (1 << irq);
		outb(MASTER_PORT_DATA, cur_mask & 0xFF);
	} else {
		irq = irq - 8;
		cur_mask = inb(SLAVE_PORT_DATA);
		cur_mask |= (1 << irq);
		outb(SLAVE_PORT_DATA, cur_mask & 0xFF);
	}
	return 0;
}

void pic_send_eoi(uint32 irq)
{
	if (irq >= 8) {
		outb(SLAVE_PORT_COMMAND, EOI);
	}
	outb(MASTER_PORT_COMMAND, EOI);
}
