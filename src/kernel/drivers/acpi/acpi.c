#include "acpi.h"
#include "../../types.h"
#include "../../utils.h"
#include "../ports/ports.h"
#include "../screen/screen.h"

typedef uint8 byte;

// check if the given address has a valid header
uint32 *acpiCheckRSDPtr(uint32 *ptr)
{
      char *sig = "RSD PTR";
      struct RSDPtr *rsdp = (struct RSDPtr *)ptr;
      byte *bptr;
      byte check = 0;
      uint32 i;

      if (memcmp(sig, rsdp, 7) == 0)
      {
            // check checksum rsdpd
            bptr = (byte *)ptr;
            for (i = 0; i < sizeof(struct RSDPtr); i++)
            {
                  check += *bptr;
                  bptr++;
            }

            // found valid rsdpd
            if (check == 0)
            {
                  if (rsdp->Revision == 0)
                        print_string("\nACPI 1.0 Found\n");
                  else
                        print_string("\nACPI 2.0 Found\n");

                  return (uint32 *)rsdp->RsdtAddress;
            }
      }

      return NULL;
}

// finds the acpi header and returns the address of the rsdt
uint32 *acpiGetRSDPtr(void)
{
      uint32 *addr;
      uint32 *rsdp;

      // search below the 1mb mark for RSDP signature
      for (addr = (uint32 *)0x000E0000; (int)addr < 0x00100000; addr++) //= 0x10/sizeof(addr))
      {
            rsdp = acpiCheckRSDPtr(addr);
            if (rsdp != NULL)
                  return rsdp;
      }

      // at address 0x40:0x0E is the RM segment of the ebda
      int ebda = *((short *)0x40E);    // get pointer
      ebda = ebda * 0x10 & 0x000FFFFF; // transform segment into linear address

      // search Extended BIOS Data Area for the Root System Description Pointer signature
      for (addr = (uint32 *)ebda; (int)addr < ebda + 1024; addr += 0x10 / sizeof(addr))
      {
            rsdp = acpiCheckRSDPtr(addr);
            if (rsdp != NULL)
                  return rsdp;
      }
      print_string("\nNO RSDPTR FOUND!!!");
      return NULL;
}

// checks for a given header and validates checksum
int acpiCheckHeader(uint32 *ptr, char *sig)
{
      if (memcmp(ptr, sig, 4) == 0)
      {
            char *checkPtr = (char *)ptr;
            int len = *(ptr + 1);
            char check = 0;
            while (0 < len--)
            {
                  check += *checkPtr;
                  checkPtr++;
            }
            if (check == 0)
                  return 0;
      }
      return -1;
}

int acpiEnable(void)
{
      // check if acpi is enabled
      if ((inw((uint32)PM1a_CNT) & SCI_EN) == 0)
      {
            // check if acpi can be enabled
            if (SMI_CMD != 0 && ACPI_ENABLE != 0)
            {
                  outb((uint32)SMI_CMD, ACPI_ENABLE); // send acpi enable command
                  // give 3 seconds time to enable acpi
                  int i;
                  for (i = 0; i < 300; i++)
                  {
                        if ((inw((uint32)PM1a_CNT) & SCI_EN) == 1)
                              break;
                        sleep(10);
                  }
                  if (PM1b_CNT != 0)
                        for (; i < 300; i++)
                        {
                              if ((inw((uint32)PM1b_CNT) & SCI_EN) == 1)
                                    break;
                              sleep(10);
                        }
                  if (i < 300)
                  {
                        print_string("enabled acpi.\n");
                        return 0;
                  }
                  else
                  {
                        print_string("couldn't enable acpi.\n");
                        return -1;
                  }
            }
            else
            {
                  print_string("no known way to enable acpi.\n");
                  return -1;
            }
      }
      else
      {
            print_string("acpi was already enabled.\n");
            return 0;
      }
      print_string("done");
}

int init_acpi(void)
{
      uint32 *ptr = acpiGetRSDPtr();
      // check if address is correct  ( if acpi is available on this pc )
      if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0)
      {
            // the RSDT contains an unknown number of pointers to acpi tables
            int entrys = *(ptr + 1);
            entrys = (entrys - 36) / 4;
            ptr += 9; // skip header information

            while (0 < entrys--)
            {
                  // check if the desired table is reached
                  if (acpiCheckHeader((uint32 *)*ptr, "FACP") == 0)
                  {
                        entrys = -2;
                        struct FACP *facp = (struct FACP *)*ptr;
                        if (acpiCheckHeader((uint32 *)facp->DSDT, "DSDT") == 0)
                        {
                              // search the \_S5 package in the DSDT
                              char *S5Addr = (char *)facp->DSDT + 36; // skip header
                              int dsdtLength = *(facp->DSDT + 1) - 36;
                              while (0 < dsdtLength--)
                              {
                                    if (memcmp(S5Addr, "_S5_", 4) == 0)
                                          break;
                                    S5Addr++;
                              }
                              // check if \_S5 was found
                              if (dsdtLength > 0)
                              {
                                    // check for valid AML structure
                                    if ((*(S5Addr - 1) == 0x08 || (*(S5Addr - 2) == 0x08 && *(S5Addr - 1) == '\\')) && *(S5Addr + 4) == 0x12)
                                    {
                                          S5Addr += 5;
                                          S5Addr += ((*S5Addr & 0xC0) >> 6) + 2; // calculate PkgLength size

                                          if (*S5Addr == 0x0A)
                                                S5Addr++; // skip byteprefix
                                          SLPYPa = *(S5Addr) << 10;
                                          S5Addr++;

                                          if (*S5Addr == 0x0A)
                                                S5Addr++; // skip byteprefix
                                          SLPYPb = *(S5Addr) << 10;

                                          SMI_CMD = facp->SMI_CMD;

                                          ACPI_ENABLE = facp->ACPI_ENABLE;
                                          ACPI_DISABLE = facp->ACPI_DISABLE;

                                          PM1a_CNT = facp->PM1a_CNT_BLK;
                                          PM1b_CNT = facp->PM1b_CNT_BLK;

                                          PM1_CNT_LEN = facp->PM1_CNT_LEN;

                                          SLP_EN = 1 << 13;
                                          SCI_EN = 1;

                                          return 0;
                                    }
                                    else
                                    {
                                          print_string("\\_S5 parse error.\n");
                                    }
                              }
                              else
                              {
                                    print_string("\\_S5 not present.\n");
                              }
                        }
                        else
                        {
                              print_string("DSDT invalid.\n");
                        }
                  }
                  ptr++;
            }
            print_string("no valid FACP present.\n");
      }
      else
      {
            print_string("no acpi.\n");
      }

      return -1;
}

void acpiPowerOff(void)
{
      // SCI_EN is set to 1 if acpi shutdown is possible
      if (SCI_EN == 0)
            return;

      acpiEnable();

      // send the shutdown command
      outports((uint32)PM1a_CNT, SLPYPa | SLP_EN);
      if (PM1b_CNT != 0)
            outports((uint32)PM1b_CNT, SLPYPb | SLP_EN);

      print_string("acpi poweroff failed.\n");
}