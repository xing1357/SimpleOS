#include "console.h"
#include "gui.h"

void draw_taskbar(){
	printf("DESKTOP");
        console_gotoxy(80,20);
        printf("________________________________________________________________________________");
}

void draw_desktop(){
	console_init(COLOR_WHITE, COLOR_BLUE);
	draw_taskbar();
}


