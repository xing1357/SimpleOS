#include "shell.h"
#include "cpu/cpuid/cpuid.h"

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
		print_string("SimpleOS (");
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
	    	panic();
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



