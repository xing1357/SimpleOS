#ifndef CPUID_H
#define CPUID_H

#include "../../drivers/screen/screen.h"
#include "../../types.h"

void cpuid(uint32 value, uint32 *eax, uint32 *ebx, unsigned int *ecx, uint32 *edx);
void print_eax(uint32 eax);
void print_ebx(uint32 ebx);
void print_edx(uint32 edx);
void cpuid_test();

#endif