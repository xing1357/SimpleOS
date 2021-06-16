/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "rtc.h"
#include "../ports/ports.h"
#include "../screen/screen.h"

unsigned char readByte(unsigned char offset)
{
    outb(0x70, offset);
    return inb(0x71);
}
void writeByte(unsigned char offset, unsigned char value)
{
    outb(0x70, offset);
    outb(0x71, value);
}

#define Year readByte(0x9) + 2000
#define Month readByte(0x8)
#define Day readByte(0x7)
#define Hour readByte(0x4)
#define Minute readByte(0x2)
#define Second readByte(0x0)

void rtc_date(){
	print_string("\n");
	print_int(Month);
	print_string("/");
	print_int(Day);
	print_string("/");
	print_int(Year);
	print_string("\n");
}

void rtc_time(){
	print_string("\n");
	print_int(Hour);
	print_string(":");
	print_int(Minute);
	print_string(":");
	print_int(Second);
	print_string("\n");
}