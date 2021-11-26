#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "mouse.h"
#include "multiboot.h"
#include "gui.h"
#include "hd.h"
#include "ext2.h"

void kmain() {
    gdt_init();
    idt_init();

    console_init(COLOR_WHITE, COLOR_BLACK);
    vga_disable_cursor();
    printf("Welcome to SimpleOS!\n");
    keyboard_init();
    //draw_desktop();
    //mouse_init();
    //read_superblock
    read_superblock();
    // Dump ext2 fs info
    dump_fs_info();
    printf("\nEND OF KERNEL!");
    
}



