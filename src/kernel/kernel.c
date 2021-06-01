#include "drivers/screen/screen.h"
#include "drivers/keyboard/keyboard.h"
#include "shell.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "fs/fs.h"
#include "fs/initrd.h"
#include "multiboot.h"


void kernel_entry(struct multiboot *mboot_ptr)
{
	uint32 initrd_location = *((uint32 *) mboot_ptr->mods_addr);
  	init_vga(WHITE, BLACK);
  	init_gdt();
  	print_string("GDT Initialised\n");
  	init_idt();
  	print_string("IDT Initialised\n");
  	fs_root = initialise_initrd (initrd_location);
  	print_string("Loaded Initial RAMDISK\n");
  	print_string("Welcome to SimpleOS!\nPlease enter a command\n");
  	print_string("Enter 'help' for commands\n");
  	launch_shell(0);

}