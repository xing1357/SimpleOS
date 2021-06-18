/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "screen.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

int cursorX = 0, cursorY = 0;

uint32 vga_index;
uint16 cursor_pos = 0, cursor_next_line_index = 1;
static uint32 next_line_index = 1;
uint8 g_fore_color = WHITE, g_back_color = BLACK;
const uint8 sw = 80,sh = 25,sd = 2;
int color = 0x0f;

int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color) 
{
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}


void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color)
{
  uint32 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index = 0;
}

void enable_cursor(uint8 cursor_start, uint8 cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}


void init_vga(uint8 fore_color, uint8 back_color)
{
  vga_buffer = (uint16*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
  enable_cursor(0,15);
}

void clearLine(uint8 from,uint8 to)
{
        uint16 i = sw * from * sd;
        string vidmem=(string)0xb8000;
        for(i;i<(sw*to*sd);i++)
        {
                vidmem[(i / 2)*2 + 1 ] = color ;
                vidmem[(i / 2)*2 ] = 0;
        }
}

void updateCursor()
{
    unsigned temp;

    temp = cursorY * sw + cursorX-1;                                                      // Position = (y * width) +  x

    outb(0x3D4, 14);                                                                // CRT Control Register: Select Cursor Location
    outb(0x3D5, temp >> 8);                                                         // Send the high byte across the bus
    outb(0x3D4, 15);                                                                // CRT Control Register: Select Send Low byte
    outb(0x3D5, temp);                                                              // Send the Low byte of the cursor location
}

void scrollUp(uint8 lineNumber)
{
        string vidmem = (string)0xb8000;
        uint16 i = 0;
        clearLine(0,lineNumber-1);                                            //updated
        for (i;i<sw*(sh-1)*2;i++)
        {
                vidmem[i] = vidmem[i+sw*2*lineNumber];
        }
        clearLine(sh-1-lineNumber,sh-1);
        if((cursorY - lineNumber) < 0 ) 
        {
                cursorY = 0;
                cursorX = 0;
        } 
        else 
        {
                cursorY -= lineNumber;
        }
        updateCursor();
}

void newLineCheck()
{
        if(cursorY >=sh-1)
        {
                scrollUp(1);
        }
}

void print_new_line()
{
  if(next_line_index >= 55){
    next_line_index = 0;
    clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
  }
  vga_index = 80*next_line_index;
  next_line_index++;
}


void clearScreen()
{
  clearLine(0,sh-1);
  cursorX = 0;
  cursorY = 0;
  updateCursor();
}

void print_char(char c)
{
    string vidmem = (string) 0xb8000;     
    switch(c)
    {
        case (0x08):
                if(cursorX > 0) 
                {
                  cursorX--;                  
                        vidmem[(cursorY * sw + cursorX)*sd]=0;       //(0xF0 & color)                          
          }
          break;
       /* case (0x09):
                cursorX = (cursorX + 8) & ~(8 - 1); 
                break;*/
        case ('\r'):
                cursorX = 0;
                break;
        case ('\n'):
                cursorX = 0;
                cursorY++;
                break;
        case ('\t'):
              cursorX = cursorX + 8;
              break;  

        default:
                vidmem [((cursorY * sw + cursorX))*sd] = c;
                vidmem [((cursorY * sw + cursorX))*sd+1] = color;
                cursorX++; 
                break;
  
    }
    if(cursorX >= sw)                                                                   
    {
        cursorX = 0;                                                                
        cursorY++;                                                                    
    }
    updateCursor();
    newLineCheck();
}


void print_string(char *str)
{
  uint32 index = 0;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
}

void print_int(int num)
{
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_string(str_num);
}

void print_string_colored(char *str, uint8 fore_color, uint8 back_color)
{
  uint32 index = 0;
  uint8 fc, bc;
  fc = g_fore_color;
  bc = g_back_color;
  g_fore_color = fore_color;
  g_back_color = back_color;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
  g_fore_color = fc;
  g_back_color = bc;
}

void print_binary(uint32 num)
{
  char bin_arr[32];
  uint32 index = 31;
  uint32 i;
  while (num > 0){
    if(num & 1){
      bin_arr[index] = '1';
    }else{
      bin_arr[index] = '0';
    }
    index--;
    num >>= 1;
  }

  for(i = 0; i < 32; ++i){
   if(i <= index)
      print_char('0');
   else
     print_char(bin_arr[i]);
  }
}




