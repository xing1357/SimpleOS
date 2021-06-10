#include "shell.h"
#include "cpu/cpuid/cpuid.h"

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
    name = file_get(i);
    if (name != FILE_NOT_FOUND)
    {
      print_string(name);
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
		print_string("User@SimpleOS (");
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
	    else
	    {
	       print_string("\n");
	       print_string(ch);
	       print_string(": ");
		   print_string("Command not found\n\n");
	    } 
	} while (!strcmp(ch,"exit"));
}



