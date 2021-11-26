#ifndef HD_H
#define HD_H

#include "types.h"
void read_sectors_ATA_PIO(uint32 target_address, uint32 LBA, uint8 sector_count);
void write_sectors_ATA_PIO(uint32 LBA, uint8 sector_count, uint32* bytes);

#endif
