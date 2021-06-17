/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "../apps.h"
#include "slang.h"

int slang_app(){
    print_string("\nEnter A slang file: ");
    string filename = readStr();
    char* str = (char*) malloc(file_size(filename));
    int response = file_read(filename, str);
    if (response == FILE_NOT_FOUND)
    {
        print_string("File not found\n");
    }
    uint32 index = 0;
    int x = 0;
    while(str[index]){
        switch(str[index]){
            case '[':
                x++;
                index++;
                break;
            
            case ']':
                x--;
                index++;
                break;
            
            case '1':
                x = x + 1;
                index++;
                break;
            
            case '2':
                x = x + 2;
                index++;
                break;
            
            case '3':
                x = x + 3;
                index++;
                break;
            
            case '4':
                x = x + 4;
                index++;
                break;
            
            case '5':
                x = x + 5;
                index++;
                break;
            
            case '6':
                x = x + 6;
                index++;
                break;
            
            case '7':
                x = x + 7;
                index++;
                break;
            
            case '8':
                x = x + 8;
                index++;
                break;
            
            case '9':
                x = x + 9;
                index++;
                break;
            
            default:
                print_string("\nSyntax Error!\n");
        }
     }
     kfree(str);
     kfree(filename);
     print_string("\nOutput: ");
     print_int(x);
     print_string("\n");
}

