#include "hd.h"
#include "io_ports.h"

/*
 BSY: a 1 means that the controller is busy executing a command. No register should be accessed (except the digital output register) while this bit is set.
RDY: a 1 means that the controller is ready to accept a command, and the drive is spinning at correct speed..
WFT: a 1 means that the controller detected a write fault.
SKC: a 1 means that the read/write head is in position (seek completed).
DRQ: a 1 means that the controller is expecting data (for a write) or is sending data (for a read). Don't access the data register while this bit is 0.
COR: a 1 indicates that the controller had to correct data, by using the ECC bytes (error correction code: extra bytes at the end of the sector that allows to verify its integrity and, sometimes, to correct errors).
IDX: a 1 indicates the the controller retected the index mark (which is not a hole on hard-drives).
ERR: a 1 indicates that an error occured. An error code has been placed in the error register.
*/

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

void ATA_wait_BSY()   //Wait for bsy to be 0
{
	while(inportb(0x1F7)&STATUS_BSY);
}
void ATA_wait_DRQ()  //Wait fot drq to be 1
{
	while(!(inportb(0x1F7)&STATUS_RDY));
}

//This is really specific to out OS now, assuming ATA bus 0 master 
//Source - OsDev wiki
void read_sectors_ATA_PIO(uint32 target_address, uint32 LBA, uint8 sector_count)
{

	ATA_wait_BSY();
	outportb(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	outportb(0x1F2,sector_count);
	outportb(0x1F3, (uint8) LBA);
	outportb(0x1F4, (uint8)(LBA >> 8));
	outportb(0x1F5, (uint8)(LBA >> 16)); 
	outportb(0x1F7,0x20); //Send the read command

	uint16 *target = (uint16*) target_address;

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++)
			target[i] = inports(0x1F0);
		target+=256;
	}
}

void write_sectors_ATA_PIO(uint32 LBA, uint8 sector_count, uint32* bytes)
{
	ATA_wait_BSY();
	outportb(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	outportb(0x1F2,sector_count);
	outportb(0x1F3, (uint8) LBA);
	outportb(0x1F4, (uint8)(LBA >> 8));
	outportb(0x1F5, (uint8)(LBA >> 16)); 
	outportb(0x1F7,0x30); //Send the write command

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
