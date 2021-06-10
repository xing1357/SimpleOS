#include "ata.h"

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

void read_sectors_ATA_PIO(uint32 target_address, uint32 LBA, uint8 sector_count)
{

	ATA_wait_BSY();
	outb(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	outb(0x1F2,sector_count);
	outb(0x1F3, (uint8) LBA);
	outb(0x1F4, (uint8)(LBA >> 8));
	outb(0x1F5, (uint8)(LBA >> 16)); 
	outb(0x1F7,0x20); //Send the read command

	uint16 *target = (uint16*) target_address;

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++)
			target[i] = inw(0x1F0);
		target+=256;
	}
}

void write_sectors_ATA_PIO(uint32 LBA, uint8 sector_count, uint32* bytes)
{
	ATA_wait_BSY();
	outb(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	outb(0x1F2,sector_count);
	outb(0x1F3, (uint8) LBA);
	outb(0x1F4, (uint8)(LBA >> 8));
	outb(0x1F5, (uint8)(LBA >> 16)); 
	outb(0x1F7,0x30); //Send the write command

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++)
		{
			outportl(0x1F0, bytes[i]);
		}
	}
}

static void ATA_wait_BSY()   //Wait for bsy to be 0
{
	while(inb(0x1F7)&STATUS_BSY);
}
static void ATA_wait_DRQ()  //Wait fot drq to be 1
{
	while(!(inb(0x1F7)&STATUS_RDY));
}