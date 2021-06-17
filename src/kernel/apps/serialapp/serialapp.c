/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "serialapp.h"
#include "../apps.h"
#include "../../drivers/serial/serial.h"

void serial_port_app(){
    clearScreen();
    print_string("\nwrite: write byte to serial port\nread: read byte from serial port\nenter 'quit' to quit");
    while(1){
        print_string("\nSerial> ");
        string cmd = readStr();
        if(strcmp(cmd, "write")){
            print_string("\nchar to write: ");
            char ch = readStr();
            write_serial(ch);
        }
        else if(strcmp(cmd, "read")){
            string reading = read_serial();
            print_string("\n");
            print_string("Reading: ");
            print_string(reading);
            print_string("\n");
        }
        else if(strcmp(cmd, "q")){
            clearScreen();
            break;
        }
        else {
            clearScreen();
            break;
        }
    }
}