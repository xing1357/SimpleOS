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
	fsinit();
  	mouse_init();
  	print_string("Initialised Mouse Driver\n");
  	print_string("Welcome to SimpleOS!\nPlease enter a command\n");
  	print_string("Enter 'help' for commands\n");
  	launch_shell(0);
}