#ifndef MOUSE_h
#define MOUSE_h

#include "../../types.h"
#include "../ports/ports.h"
#include "../screen/screen.h"

volatile int mousex=512,mousey=384;
volatile int mouse_left_click = 0;
volatile int mouse_right_click = 0;

volatile int mousedeltax=0,mousedeltay=0;
volatile char mouse_bytes[3];
static uint32 cycle = 0;
uint8* mouse_buff = 0;
int mouse_lclick_duration = 0;

void mouse_wait(unsigned char type);

void mouse_write(unsigned char a_write);

uint8 mouse_read();

void mouse_init();

void mouse_handler();

#endif