#include "panic.h"

void panic(){
	asm volatile("cli");
	print_string("\nPANIC: SYSTEM HALTING\n");
	asm volatile("hlt");
}

