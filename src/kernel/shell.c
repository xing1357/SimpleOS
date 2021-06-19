/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "shell.h"
#include "cpu/cpuid/cpuid.h"
#include "apps/textedit/textedit.h"
#include "apps/calculator/calculator.h"
#include "apps/serialapp/serialapp.h"
#include "apps/slang/slang.h"

void read_file()
{
  // get the filename
  print_string("\nEnter file name: ");
  string filename = readStr();
  // read it
  char* content = (char*) malloc(file_size(filename));
  int response = file_read(filename, content);
  if (response == FILE_NOT_FOUND)
  {
    print_string("File not found\n");
  }
  print_string("\n");
  print_string(content);
  print_string("\n");
  kfree(filename);
  kfree(content);
}

void run(){
	print_string("\nEnter App to Run: ");
	string app = readStr();
	if(strcmp(app, "textedit")){
		text_editor();
	}
	else if(strcmp(app, "calc")){
		calculator();
	}
	else if(strcmp(app, "serialapp")){
		serial_port_app();
	}
	else if(strcmp(app, "slang")){
		slang_app();
	}
	else {
		print_string("\nApp Not Found\nAvailable Applications:\n1. textedit\n2. calc\n3. serialapp\n4. slang");
	}
}

void rmfile(){
	print_string("\nEnter File to Remove: ");
	string filename = readStr();
	file_remove(filename);
	print_string("\nRemoved File: ");
	print_string(filename);
	print_string("\n");
}

void ls()
{
  char* name;
  for (int i = 0; i < file_count(); ++i)
  {
    name = file_get_name(i);
    if (name != FILE_NOT_FOUND)
    {
      print_string(name);
	  int size = file_size(name);
	  print_string("\t");
	  print_int(size);
      print_string("\n");
    }
  }
}
void mkfile(){
	print_string("\nEnter Filename: ");
	string filename = readStr();
	file_make(filename);
	print_string("\nCreated File: ");
	print_string(filename);
	print_string("\n");
}

void echo()
{
	print_string("\n");
	string str = readStr();
	print_string("\n");
	print_string(str);
	print_string("\n");
}


void help()
{
	print_string("\nAbout: About the OS\n");
	print_string("cmd: open a new shell\n");
	print_string("cpuinfo: Information about the cpu\n");
	print_string("echo: echo text into the terminal\n");
	print_string("rand: Random Integer Between 1 and 100\n");
	print_string("cls: Clear the Screen\n");
	print_string("page: Test Paging\n");
	print_string("panic: TRIGGER A KERNEL PANIC!!!\n");
	print_string("date: Print Dage\n");
	print_string("time: Print Time\n");
	print_string("ls: List the Files\n");
	print_string("mkfile: Create a file\n");
	print_string("rmfile: Remove a file\n");
	print_string("run: SimpleOS Applauncher\n");
	print_string("shutdown: Shutdown SimpleOS\n");
}

void about()
{
	print_string("\nSimpleOS v1.0");
	print_string("\nBy Harvey Xing\n");
}

void launch_shell(int n)
{
	string ch = (string) malloc(200); 
	int counter = 0;
	do
	{
		char* username = (char*) malloc(file_size("username"));
		int response = file_read("username", username);
		print_string(username);
		print_string("@SimpleOS (");
		print_string(int_to_string(n));
		print_string(")> ");
		ch = readStr(); 
	    if(strcmp(ch,"cmd"))
	    {
            print_string("\nYou are allready in cmd. A new shell will be opened\n");
			launch_shell(n+1);
	    }

	    else if(strcmp(ch,"help"))
	    {
    		help();
	    }

	    else if(strcmp(ch,"echo"))
	    {
    		echo();
	    }
	    else if(strcmp(ch,"cpuinfo"))
	    {
	    	cpuid_test();
	    }
	    else if(strcmp(ch, "about"))
	    {
	    	about();
	    }
	    else if(strcmp(ch, "rand"))
	    {
	    	print_string("\n");
	    	string randint = int_to_string(rand(100));
	    	print_string(randint);
	    	print_string("\n");
	    }
	    else if(strcmp(ch, "cls")){
	    	clearScreen();
	    }
	    else if(strcmp(ch, "page")){
	    	print_string("\n");
	    	page();
	    }
	    else if(strcmp(ch, "panic")){
	    	panic("User");
	    }
	    else if(strcmp(ch, "date")){
	    	rtc_date();
	    }
	    else if(strcmp(ch, "time")){
	    	rtc_time();
	    }
		else if(strcmp(ch, "find")){
			string file = readStr();
			bool fileExists = file_exists(file);
			if (fileExists == true){
				print_string("\nFile Found\n");
			}
			else if(fileExists == false) {
				print_string("\nFile not Found\n");
			}
		}
		else if(strcmp(ch, "ls")){
			print_string("\n");
			ls();
			print_string("\n");
		}
		else if(strcmp(ch,"mkfile")){
			mkfile();
			print_string("\n");
		}
		else if(strcmp(ch, "rmfile")){
			rmfile();
		}
		else if(strcmp(ch, "readfile")){
			read_file();
		}
		else if(strcmp(ch, "run")){
			run();
		}
		else if(strcmp(ch, "shutdown")){
			acpiPowerOff();
		}
	    else
	    {
	       print_string("\n");
	       print_string(ch);
	       print_string(": ");
		   print_string("Command not found\n\n");
	    } 
	} while (!strcmp(ch,"exit"));
}



