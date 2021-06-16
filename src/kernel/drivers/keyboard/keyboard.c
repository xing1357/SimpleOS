/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "keyboard.h"
#include "../ports/ports.h"
#include "../screen/screen.h"

string readStr()
{
    char buff;
    string buffstr = (string) malloc(200);
    uint8 i = 0;
    uint8 reading = 1;
    while(reading)
    {
        if(inb(0x64) & 0x1)                 
        {
            switch(inb(0x60))
            { 
      
        case 2:
                print_char('1');
                buffstr[i] = '1';
                i++;
                break;
        case 3:
                print_char('2');
                buffstr[i] = '2';
                i++;
                break;
        case 4:
                print_char('3');
                buffstr[i] = '3';
                i++;
                break;
        case 5:
                print_char('4');
                buffstr[i] = '4';
                i++;
                break;
        case 6:
                print_char('5');
                buffstr[i] = '5';
                i++;
                break;
        case 7:
                print_char('6');
                buffstr[i] = '6';
                i++;
                break;
        case 8:
                print_char('7');
                buffstr[i] = '7';
                i++;
                break;
        case 9:
                print_char('8');
                buffstr[i] = '8';
                i++;
                break;
        case 10:
                print_char('9');
                buffstr[i] = '9';
                i++;
                break;
        case 11:
                print_char('0');
                buffstr[i] = '0';
                i++;
                break;
        case 12:
                print_char('-');
                buffstr[i] = '-';
                i++;
                break;
        case 13:
                print_char('=');
                buffstr[i] = '=';
                i++;
                break;
        case 14:
                print_char('\b');
                i--;
                if(i<0)
                {
                  i = 0;
                }
                buffstr[i+1] = 0;
                buffstr[i] = 0;
                break;
       /* case 15:
                print_char('\t');          Tab button
                buffstr[i] = '\t';
                i++;
                break;*/
        case 16:
                print_char('q');
                buffstr[i] = 'q';
                i++;
                break;
        case 17:
                print_char('w');
                buffstr[i] = 'w';
                i++;
                break;
        case 18:
                print_char('e');
                buffstr[i] = 'e';
                i++;
                break;
        case 19:
                print_char('r');
                buffstr[i] = 'r';
                i++;
                break;
        case 20:
                print_char('t');
                buffstr[i] = 't';
                i++;
                break;
        case 21:
                print_char('y');
                buffstr[i] = 'y';
                i++;
                break;
        case 22:
                print_char('u');
                buffstr[i] = 'u';
                i++;
                break;
        case 23:
                print_char('i');
                buffstr[i] = 'i';
                i++;
                break;
        case 24:
                print_char('o');
                buffstr[i] = 'o';
                i++;
                break;
        case 25:
                print_char('p');
                buffstr[i] = 'p';
                i++;
                break;
        case 26:
                print_char('[');
                buffstr[i] = '[';
                i++;
                break;
        case 27:
                print_char(']');
                buffstr[i] = ']';
                i++;
                break;
        case 28:
               // print_char('\n');
               // buffstr[i] = '\n';
                i++;
                reading = 0;
                break;
      /*  case 29:
                print_char('q');           Left Control
                buffstr[i] = 'q';
                i++;
                break;*/
        case 30:
                print_char('a');
                buffstr[i] = 'a';
                i++;
                break;
        case 31:
                print_char('s');
                buffstr[i] = 's';
                i++;
                break;
        case 32:
                print_char('d');
                buffstr[i] = 'd';
                i++;
                break;
        case 33:
                print_char('f');
                buffstr[i] = 'f';
                i++;
                break;
        case 34:
                print_char('g');
                buffstr[i] = 'g';
                i++;
                break;
        case 35:
                print_char('h');
                buffstr[i] = 'h';
                i++;
                break;
        case 36:
                print_char('j');
                buffstr[i] = 'j';
                i++;
                break;
        case 37:
                print_char('k');
                buffstr[i] = 'k';
                i++;
                break;
        case 38:
                print_char('l');
                buffstr[i] = 'l';
                i++;
                break;
        case 39:
                print_char(';');
                buffstr[i] = ';';
                i++;
                break;
        case 40:
                print_char((char)44);               //   Single quote (')
                buffstr[i] = (char)44;
                i++;
                break;
        case 41:
                print_char((char)44);               // Back tick (`)
                buffstr[i] = (char)44;
                i++;
                break;
     /* case 42:                                 Left shift 
                print_char('q');
                buffstr[i] = 'q';
                i++;
                break;
        case 43:                                 \ (< for somekeyboards)   
                print_char((char)92);
                buffstr[i] = 'q';
                i++;
                break;*/
        case 44:
                print_char('z');
                buffstr[i] = 'z';
                i++;
                break;
        case 45:
                print_char('x');
                buffstr[i] = 'x';
                i++;
                break;
        case 46:
                print_char('c');
                buffstr[i] = 'c';
                i++;
                break;
        case 47:
                print_char('v');
                buffstr[i] = 'v';
                i++;
                break;                
        case 48:
                print_char('b');
                buffstr[i] = 'b';
                i++;
                break;               
        case 49:
                print_char('n');
                buffstr[i] = 'n';
                i++;
                break;                
        case 50:
                print_char('m');
                buffstr[i] = 'm';
                i++;
                break;               
        case 51:
                print_char(',');
                buffstr[i] = ',';
                i++;
                break;                
        case 52:
                print_char('.');
                buffstr[i] = '.';
                i++;
                break;            
        case 53:
                print_char('/');
                buffstr[i] = '/';
                i++;
                break;            
        case 54:
                print_char('.');
                buffstr[i] = '.';
                i++;
                break;            
        case 55:
                print_char('/');
                buffstr[i] = '/';
                i++;
                break;            
      /*case 56:
                print_char(' ');           Right shift
                buffstr[i] = ' ';
                i++;
                break;*/           
        case 57:
                print_char(' ');
                buffstr[i] = ' ';
                i++;
                break;
            }
        }
    }
    buffstr[i-1] = 0;                
    return buffstr;
}

string textedit_readStr()
{
    char buff;
    string buffstr = (string) malloc(200);
    uint8 i = 0;
    uint8 reading = 1;
    while(reading)
    {
        if(inb(0x64) & 0x1)                 
        {
            switch(inb(0x60))
            {
        
        case 1:
                reading = 0;
                i++;
                break;
                    
      
        case 2:
                print_char('1');
                buffstr[i] = '1';
                i++;
                break;
        case 3:
                print_char('2');
                buffstr[i] = '2';
                i++;
                break;
        case 4:
                print_char('3');
                buffstr[i] = '3';
                i++;
                break;
        case 5:
                print_char('4');
                buffstr[i] = '4';
                i++;
                break;
        case 6:
                print_char('5');
                buffstr[i] = '5';
                i++;
                break;
        case 7:
                print_char('6');
                buffstr[i] = '6';
                i++;
                break;
        case 8:
                print_char('7');
                buffstr[i] = '7';
                i++;
                break;
        case 9:
                print_char('8');
                buffstr[i] = '8';
                i++;
                break;
        case 10:
                print_char('9');
                buffstr[i] = '9';
                i++;
                break;
        case 11:
                print_char('0');
                buffstr[i] = '0';
                i++;
                break;
        case 12:
                print_char('-');
                buffstr[i] = '-';
                i++;
                break;
        case 13:
                print_char('=');
                buffstr[i] = '=';
                i++;
                break;
        case 14:
                print_char('\b');
                i--;
                if(i<0)
                {
                  i = 0;
                }
                buffstr[i+1] = 0;
                buffstr[i] = 0;
                break;
       /* case 15:
                print_char('\t');          Tab button
                buffstr[i] = '\t';
                i++;
                break;*/
        case 16:
                print_char('q');
                buffstr[i] = 'q';
                i++;
                break;
        case 17:
                print_char('w');
                buffstr[i] = 'w';
                i++;
                break;
        case 18:
                print_char('e');
                buffstr[i] = 'e';
                i++;
                break;
        case 19:
                print_char('r');
                buffstr[i] = 'r';
                i++;
                break;
        case 20:
                print_char('t');
                buffstr[i] = 't';
                i++;
                break;
        case 21:
                print_char('y');
                buffstr[i] = 'y';
                i++;
                break;
        case 22:
                print_char('u');
                buffstr[i] = 'u';
                i++;
                break;
        case 23:
                print_char('i');
                buffstr[i] = 'i';
                i++;
                break;
        case 24:
                print_char('o');
                buffstr[i] = 'o';
                i++;
                break;
        case 25:
                print_char('p');
                buffstr[i] = 'p';
                i++;
                break;
        case 26:
                print_char('[');
                buffstr[i] = '[';
                i++;
                break;
        case 27:
                print_char(']');
                buffstr[i] = ']';
                i++;
                break;
        case 28:
                print_char('\n');
                buffstr[i] = '\n';
                i++;
                break;
      /*  case 29:
                print_char('q');           Left Control
                buffstr[i] = 'q';
                i++;
                break;*/
        case 30:
                print_char('a');
                buffstr[i] = 'a';
                i++;
                break;
        case 31:
                print_char('s');
                buffstr[i] = 's';
                i++;
                break;
        case 32:
                print_char('d');
                buffstr[i] = 'd';
                i++;
                break;
        case 33:
                print_char('f');
                buffstr[i] = 'f';
                i++;
                break;
        case 34:
                print_char('g');
                buffstr[i] = 'g';
                i++;
                break;
        case 35:
                print_char('h');
                buffstr[i] = 'h';
                i++;
                break;
        case 36:
                print_char('j');
                buffstr[i] = 'j';
                i++;
                break;
        case 37:
                print_char('k');
                buffstr[i] = 'k';
                i++;
                break;
        case 38:
                print_char('l');
                buffstr[i] = 'l';
                i++;
                break;
        case 39:
                print_char(';');
                buffstr[i] = ';';
                i++;
                break;
        case 40:
                print_char((char)44);               //   Single quote (')
                buffstr[i] = (char)44;
                i++;
                break;
        case 41:
                print_char((char)44);               // Back tick (`)
                buffstr[i] = (char)44;
                i++;
                break;
     /* case 42:                                 Left shift 
                print_char('q');
                buffstr[i] = 'q';
                i++;
                break;
        case 43:                                 \ (< for somekeyboards)   
                print_char((char)92);
                buffstr[i] = 'q';
                i++;
                break;*/
        case 44:
                print_char('z');
                buffstr[i] = 'z';
                i++;
                break;
        case 45:
                print_char('x');
                buffstr[i] = 'x';
                i++;
                break;
        case 46:
                print_char('c');
                buffstr[i] = 'c';
                i++;
                break;
        case 47:
                print_char('v');
                buffstr[i] = 'v';
                i++;
                break;                
        case 48:
                print_char('b');
                buffstr[i] = 'b';
                i++;
                break;               
        case 49:
                print_char('n');
                buffstr[i] = 'n';
                i++;
                break;                
        case 50:
                print_char('m');
                buffstr[i] = 'm';
                i++;
                break;               
        case 51:
                print_char(',');
                buffstr[i] = ',';
                i++;
                break;                
        case 52:
                print_char('.');
                buffstr[i] = '.';
                i++;
                break;            
        case 53:
                print_char('/');
                buffstr[i] = '/';
                i++;
                break;            
        case 54:
                print_char('.');
                buffstr[i] = '.';
                i++;
                break;            
        case 55:
                print_char('/');
                buffstr[i] = '/';
                i++;
                break;            
      /*case 56:
                print_char(' ');           Right shift
                buffstr[i] = ' ';
                i++;
                break;*/           
        case 57:
                print_char(' ');
                buffstr[i] = ' ';
                i++;
                break;
            }
        }
    }
    buffstr[i-1] = 0;                
    return buffstr;
}






