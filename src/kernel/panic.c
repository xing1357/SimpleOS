/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "panic.h"

void panic(char *str){
	asm volatile("cli");
	print_string("\nPANIC:");
	print_string(str);
	print_string(" SYSTEM HALTING\n");
	asm volatile("hlt");
}

