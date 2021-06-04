#ifndef SERIAL_H
#define SERIAL_H

#define PORT_COM1 0x3f8

int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial(char a);

void serial_init();

#endif