/*
Copyright 2021 Harvey Xing
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "textedit.h"
#include "../apps.h"

void text_editor(){
 	print_string("\nEnter File To Create/Edit: ");
 	string filename = readStr();
	bool exists = file_exists(filename);
	if(exists == false){
		file_make(filename);
 		clearScreen();
 		print_string("Text Editor\n");
 		string input = textedit_readStr();
 		file_writes(filename, input);
 		clearScreen();
	}

	else if(exists == true){
 		char* str = (char*) malloc(file_size(filename));
 		int response = file_read(filename, str);
		clearScreen();
        print_string(str);
 		string input = textedit_readStr();
		input = strcat(str, input);
 		file_writes(filename, input);
 		clearScreen();
	}
	else {
		print_string("\nUnrecognised Command!\n");
	}

}