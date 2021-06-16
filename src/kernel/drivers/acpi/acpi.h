/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#ifndef ACPI_h
#define ACPI_h

#include "../../types.h"

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef uint32 qword;

dword *SMI_CMD;
byte ACPI_ENABLE;
byte ACPI_DISABLE;
dword *PM1a_CNT;
dword *PM1b_CNT;
word SLPYPa;
word SLPYPb;
word SLP_EN;
word SCI_EN;
byte PM1_CNT_LEN;



struct RSDPtr
{
   byte Signature[8];
   byte CheckSum;
   byte OemID[6];
   byte Revision;
   dword *RsdtAddress;
};

struct ACPI_SDTHeader
{
  char Signature[4];
  uint32 length;
  uint8 Revision;
  uint8 Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32 OEMRevision;
  uint32 CreatorID;
  uint32 CreatorRevision;
};

struct RSDT
{
  struct ACPI_SDTHeader h;
  uint32 *PointerToOtherSDT;
};

struct FACP
{
   byte Signature[4];
   dword Length;
   byte unneded1[40 - 8];
   dword *DSDT;
   byte unneded2[48 - 44];
   dword *SMI_CMD;
   byte ACPI_ENABLE;
   byte ACPI_DISABLE;
   byte unneded3[64 - 54];
   dword *PM1a_CNT_BLK;
   dword *PM1b_CNT_BLK;
   byte unneded4[89 - 72];
   byte PM1_CNT_LEN;
};

typedef struct MADT
{
  char Signature[4];
  uint32 Length;
  uint8 revision;
  uint8 checksum;
  char OEMID[6];
  char OEMable_id[8];
  uint32 OEMRevision;
  uint32 CreatorID;
  uint32 CreatorREV;
  uint32 local_controller_addr;
  uint32 flags;
  uint8 rest_fields;
}MADT;


void acpiPowerOff(void);

int acpiCheckHeader(uint32 *ptr, char *sig);

uint32 *acpiGetRSDPtr(void);

uint32 *acpiCheckRSDPtr(uint32 *ptr);

#endif // ACPI_h