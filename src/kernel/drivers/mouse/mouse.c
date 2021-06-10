#include "mouse.h"


void mouse_wait(uint8 type)
{
  uint32 _time_out=100000;
  if(type==0)
  {
    while(_time_out--) 
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) 
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}
void mouse_write(uint8 a_write)
{
 mouse_wait(1);
 outb(0x64, 0xD4);
 mouse_wait(1);
 outb(0x60, a_write);
}

uint8 mouse_read()
{
 mouse_wait(0);
 return inb(0x60);
}

void mouse_init()
{
    mouse_wait(1);
    outb(0x64,0xA8);
    mouse_wait(1);
    outb(0x64,0x20);
    uint8 status_byte;
    mouse_wait(0);
    status_byte = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status_byte);
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
}

void mouse_handler()
{
 while(1)
  {
    while(cycle<3)
      mouse_bytes[cycle++] = inb(0x60);
    { 
      cycle = 0; 
      if ((mouse_bytes[0] & 0x80) || (mouse_bytes[0] & 0x40))
        return; 
      if (!(mouse_bytes[0] & 0x20))
        mousedeltay |= 0xFFFFFF00; 
      if (!(mouse_bytes[0] & 0x10))
        mousedeltax |= 0xFFFFFF00; 
      mousedeltax=mouse_bytes[1];
      mousedeltay=mouse_bytes[2];
    }
    asm volatile("int $50");
  }
}