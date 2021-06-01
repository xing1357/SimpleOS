#ifndef SCREEN_H
#define SCREEN_H

#include "../../types.h"
#include "../../utils.h"
#include "../ports/ports.h"

#define VGA_ADDRESS 0xB8000
#define BUFSIZE 2200

uint16* vga_buffer;

#define NULL 0

enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

#define BOX_MAX_WIDTH 78
#define BOX_MAX_HEIGHT 23

#define BOX_SINGLELINE 1
#define BOX_DOUBLELINE 2


uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color);
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color);
void init_vga(uint8 fore_color, uint8 back_color);
void print_new_line();
void scroll_up();
void print_char(char ch);
void print_new_line();
void print_string(char *str);
void print_int(int num);
void clearScreen();
void print_string_colored(char *str, uint8 fore_color, uint8 back_color);
void print_binary(uint32 num);

#endif