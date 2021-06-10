#include "ports.h"

uint8 inb(uint16 port)
{
  uint8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
  return ret;
}

void outb(uint16 port, uint8 data)
{
  asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}

uint16 inports(uint16 _port) {
    uint16 rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

uint16 inw (uint16 port) {
    uint16 result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void outports(uint16 _port, uint16 _data) {
    asm volatile ("outw %1, %0" : : "dN" (_port), "a" (_data));
}

uint32 inportl(uint16 _port) {
    uint32 rv;
    asm volatile ("inl %%dx, %%eax" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportl(uint16 _port, uint32 _data) {
    asm volatile ("outl %%eax, %%dx" : : "dN" (_port), "a" (_data));
}


void wait_for_io(uint32 timer_count) 
{
  while(1){
    asm volatile("nop");
    timer_count--;
    if(timer_count <= 0)
      break;
    }
}

void sleep(uint32 timer_count)
{
  wait_for_io(timer_count);
}