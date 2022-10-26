#include "panic.h"
#include "console.h"

void panic(char *message){
    printf("\nKernel Panic! System Halting\nReason: %s\n", message);
    asm volatile("hlt");
}