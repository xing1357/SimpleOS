/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#ifndef MOUSE_h
#define MOUSE_h

#include "../../types.h"
#include "../ports/ports.h"
#include "../screen/screen.h"

typedef struct register_t {
	/// FS and GS have no hardware-assigned uses.
	uint32 gs;
	/// FS and GS have no hardware-assigned uses.
	uint32 fs;
	/// Extra Segment determined by the programmer.
	uint32 es;
	/// Data Segment.
	uint32 ds;
	/// 32-bit destination register.
	uint32 edi;
	/// 32-bit source register.
	uint32 esi;
	/// 32-bit base pointer register.
	uint32 ebp;
	/// 32-bit stack pointer register.
	uint32 esp;
	/// 32-bit base register.
	uint32 ebx;
	/// 32-bit data register.
	uint32 edx;
	/// 32-bit counter.
	uint32 ecx;
	/// 32-bit accumulator register.
	uint32 eax;
	/// Interrupt number.
	uint32 int_no;
	/// Error code.
	uint32 err_code;
	/// Instruction Pointer Register.
	uint32 eip;
	/// Code Segment.
	uint32 cs;
	/// 32-bit flag register.
	uint32 eflags;
	// TODO: Check meaning!
	uint32 useresp;
	/// Stack Segment.
	uint32 ss;
} register_t;

#define MOUSE_ENABLE_PACKET         0xF4

/// The mouse stops sending automatic packets.
#define MOUSE_DISABLE_PACKET        0xF5

/// Disables streaming, sets the packet rate to 100 per second, and
/// resolution to 4 pixels per mm.
#define MOUSE_USE_DEFAULT_SETTINGS  0xF6

/// @brief Sets up the mouse by installing the mouse handler into IRQ12.
void mouse_init();

/// @brief Enable the mouse driver.
void mouse_enable();

/// @brief Disable the mouse driver.
void mouse_disable();

/// @brief      Mouse wait for a command.
/// @param type 1 for sending - 0 for receiving.
void mouse_waitcmd(unsigned char type);

/// @brief      Send data to mouse.
/// @param data The data to send.
void mouse_write(unsigned char data);

/// @brief  Read data from mouse.
/// @return The data received from mouse.
unsigned char mouse_read();

/// @brief The mouse handler.
void mouse_isr(register_t *r);

#endif