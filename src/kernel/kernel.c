/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "drivers/screen/screen.h"
#include "drivers/keyboard/keyboard.h"
#include "shell.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "fs/vfs.h"
#include "fs/initrd/initrd.h"
#include "multiboot.h"
#include "drivers/serial/serial.h"
#include "drivers/pci/pci.h"
#include "drivers/mouse/mouse.h"
#include "fs/fs.h"
#include "drivers/acpi/acpi.h"
#include "fs/tar.h"
#include "printf.h"

void kernel_entry(struct multiboot *mboot_ptr)
{
	uint32 initrd_location = *((uint32 *) mboot_ptr->mods_addr);
  	init_vga(WHITE, BLACK);
  	init_gdt();
  	print_string("GDT Initialised\n");
  	init_idt();
  	print_string("IDT Initialised\n");
  	serial_init();
  	print_string("Serial  Driver Initialised\n");
  	pci_init();
  	print_string("PCI Driver Initialised\n");
	init_acpi();
	print_string("Initialised ACPI\n");
	fs_root = initialise_initrd(initrd_location);
	if (!fs_root){
		panic("RAMDISK error");
	}
	else {
		print_string("Loaded Initial RAMDISK\n");
	}
	int tarfound = parse(initrd_location);
	print_string("\nFiles Found in RAMDISK: ");
	print_int(tarfound);
	print_string("\n");
	fsinit();
	print_string("Initialised the Filesystem\n");
  	mouse_init();
  	print_string("Initialised Mouse Driver\n");
	printf("%s","Printf() Test\n");
	print_string("\nSimpleOS Setup\n");
	print_string("Enter A Username: ");
	bool setup_done = file_exists("username");
	if(setup_done == true){
		launch_shell(0);
	}
	else {
		string username = readStr();
		file_make("username");
		file_writes("username", username);
 		print_string("\nWelcome to SimpleOS!\nPlease enter a command\n");
  		print_string("Enter 'help' for commands\n");
  		launch_shell(0);
	}
}

