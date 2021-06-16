/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "textedit.h"
#include "../apps.h"

void text_editor(){
	print_string("\nEnter File To Create: ");
	string filename = readStr();
	file_make(filename);
	clearScreen();
    print_string("Text Editor\n");
	string input = textedit_readStr();
	file_writes(filename, input);
	clearScreen();

}