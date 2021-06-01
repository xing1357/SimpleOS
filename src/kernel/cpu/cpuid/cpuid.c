#include "cpuid.h"

void cpuid(uint32 value, uint32 *eax, uint32 *ebx, unsigned int *ecx, uint32 *edx)
{
  uint32 eaxres, ebxres, ecxres, edxres;
  asm("xorl\t%eax, %eax");
  asm("xorl\t%ebx, %ebx");
  asm("xorl\t%ecx, %ecx");
  asm("xorl\t%edx, %edx");
  asm("movl\t%0, %%eax":"=m"(value));
  asm("cpuid");
  asm("movl\t%%eax, %0" : "=m" (eaxres));
  asm("movl\t%%ebx, %0" : "=m" (ebxres));
  asm("movl\t%%ecx, %0" : "=m" (ecxres));
  asm("movl\t%%edx, %0" : "=m" (edxres));
  *eax = eaxres;
  *ebx = ebxres;
  *ecx = ecxres;
  *edx = edxres;    
}

void print_eax(uint32 eax)
{
  uint32 step_id, model, family_id, proc_type, ext_mod_id, ext_fam_id;
  step_id = model = family_id = proc_type = ext_mod_id = ext_fam_id = eax;
  
  step_id &= (2 << 3) - 1;	//bits 0-3
  model >>= 4;	//bits 4-7
  model &= (2 << 3) - 1;
  family_id >>= 8;	//bits 8-11
  family_id &= (2 << 3) - 1;
  proc_type >>= 12;	//bits 12-13
  proc_type &= (2 << 1) - 1;
  ext_mod_id >>= 16;	//bits 16-19
  ext_mod_id &= (2 << 3) - 1;
  ext_fam_id >>= 20;	//bits 20-27
  ext_fam_id &= (2 << 7) - 1;

  print_string("\nEAX :-");
  print_string("\n\tStepping ID: ");
  print_int(step_id);
  print_string("\n\tModel: ");
  print_int(model);
  print_string("\n\tFamily ID: ");
  print_int(family_id);
  print_string("\n\tProcessor Type: ");
  print_int(proc_type);
  print_string("\n\tExtended Model ID: ");
  print_int(ext_mod_id);
  print_string("\n\tExtended Family ID: ");
  print_int(ext_fam_id);
}

void print_ebx(uint32 ebx)
{
  uint32 brand_index, cache_line_size, max_addr_id, init_apic_id;
  brand_index = cache_line_size = max_addr_id = init_apic_id = 0;
  char *bytes = (char*)&ebx;

  brand_index = bytes[0];	//bits 0-7
  cache_line_size = bytes[1];	//bits 8-15
  max_addr_id = bytes[2];	//bits 16-23
  init_apic_id = bytes[3];	//bits 24-31

  print_string("\nEBX :-");
  print_string("\n\tBrand Index: ");
  print_int(brand_index);
  print_string("\n\tCache Line Size: ");
  print_int(cache_line_size);
  print_string("\n\tMax Addressable ID for Logical Processors: ");
  print_int(max_addr_id);
  print_string("\n\tInitial APIC ID: ");
  print_int(init_apic_id);
}

void print_edx(uint32 edx)
{
  print_string("\nEDX :-");
  print_string("\n\tbit-31 [ ");
  print_binary(edx);
  print_string(" ] bit-0");
  print_string("\n\tBit 0 : FPU-x87 FPU on Chip");
  print_string("\n\tBit 1 : VME-Virtual-8086 Mode Enhancement");
  print_string("\n\tBit 2 : DE-Debugging Extensions");
  print_string("\n\tBit 3 : PSE-Page Size Extensions");
  print_string("\n\tBit 4 : TSC-Time Stamp Counter");
  print_string("\n\tBit 5 : MSR-RDMSR and WRMSR Support");
  print_string("\n\tBit 6 : PAE-Physical Address Extensions\n");
}

void cpuid_test()
{
  uint32 eax, ebx, ecx, edx;

  print_string("\nCPUID EAX = 01H");

  //see cpuid instruction in Intel Manual for more information
  cpuid(0x01, &eax, &ebx, &ecx, &edx);

  print_eax(eax);
  print_ebx(ebx);
  print_edx(edx);
}