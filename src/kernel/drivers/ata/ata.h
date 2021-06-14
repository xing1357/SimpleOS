#ifndef ATA_H
#define ATA_H

#include "../ports/ports.h"
#include "../../types.h"

static void ATA_wait_BSY();
static void ATA_wait_DRQ();

void read_sectors_ATA_PIO(uint32 target_address, uint32 LBA, uint8 sector_count);
void write_sectors_ATA_PIO(uint32 LBA, uint8 sector_count, uint32* bytes);


#endif
