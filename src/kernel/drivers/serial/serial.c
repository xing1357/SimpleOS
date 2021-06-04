#include "../ports/ports.h"
#include "serial.h"

int serial_received() {
   return inb(PORT_COM1 + 5) & 1;
}

char read_serial() {
   while (serial_received() == 0);
   return inb(PORT_COM1);
}

// Send

int is_transmit_empty() {
   return inb(PORT_COM1 + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);
   outb(PORT_COM1,a);
}


void serial_init() {
   outb(PORT_COM1 + 1, 0x00);
   outb(PORT_COM1 + 3, 0x80);
   outb(PORT_COM1 + 0, 0x03);
   outb(PORT_COM1 + 1, 0x00);
   outb(PORT_COM1 + 3, 0x03);
   outb(PORT_COM1 + 2, 0xC7);
   outb(PORT_COM1 + 4, 0x0B);
}