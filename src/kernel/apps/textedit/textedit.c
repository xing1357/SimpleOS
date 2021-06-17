/*
Copyright 2021 Harvey Xing
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "textedit.h"
#include "../apps.h"

void text_editor(){
	print_string("\nWhat would you like to do?");
 	print_string("\nCreate a new file. 0\nEdit an existing File. 1\n");
 	string response = readStr();

 	if (strcmp(response, "0")) {
 		print_string("\nEnter File To Create: ");
 		string filename = readStr();
 		file_make(filename);
 		clearScreen();
 		print_string("Text Editor\n");
 		string input = textedit_readStr();
 		file_writes(filename, input);
 		clearScreen();
 	}else if (strcmp(response, "1")) {
        print_string("\nEnter A File Name: ");
 		string filename = readStr();
 		char* str = (char*) malloc(file_size(filename));
 		int response = file_read(filename, str);
 		if (response == FILE_NOT_FOUND) {
 			print_string("File not found\n");
 		}
	else {
		print_string("\nUnrecognised Command!\n");
	}

 		clearScreen();
        print_string(str);
 		string input = textedit_readStr();
		input = strcat(str, input);
 		file_writes(filename, input);
 		clearScreen();
 	}
}