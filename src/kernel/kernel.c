#include "drivers/screen/screen.h"
#include "drivers/keyboard/keyboard.h"
#include "shell.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "fs/fs.h"
#include "fs/initrd.h"
#include "multiboot.h"

void find_files(){
	int i = 0;
	struct dirent *node = 0;
	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
	  print_string("Found file ");
	  print_string(node->name);
	  fs_node_t *fsnode = finddir_fs(fs_root, node->name);

	  if ((fsnode->flags&0x7) == FS_DIRECTORY)
	    print_string("\n\t(directory)\n");
	  else
	  {
	    print_string("\n\t contents: \"");
	    char buf[256];
	    uint32 sz = read_fs(fsnode, 0, 256, buf);
	    int j;
	    for (j = 0; j < sz; j++)
	      print_char(buf[j]);

	    print_string("\"\n");
	  }
  	i++;
	}
}

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
  	find_files();
  	print_string("Welcome to SimpleOS!\nPlease enter a command\n");
  	print_string("Enter 'help' for commands\n");
  	launch_shell(0);

}