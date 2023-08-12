#ifndef FAT32_H
#define FAT32_H

#include "types.h"

typedef struct fat_bpb
{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 	    bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;
	//Fat32 Extended BPBL
	unsigned int		table_size_32;
	unsigned short		extended_flags;
	unsigned short		fat_version;
	unsigned int		root_cluster;
	unsigned short		fat_info;
	unsigned short		backup_BS_sector;
	unsigned char 		reserved_0[12];
	unsigned char		drive_number;
	unsigned char 		reserved_1;
	unsigned char		boot_signature;
	unsigned int 		volume_id;
	unsigned char		volume_label[11];
	unsigned char		fat_type_label[8];
 
}__attribute__((packed)) fat_bpb;

typedef struct 
{
    uint8 Name[11];
    uint8 Attributes;
    uint8 Reserved;
    uint8 CreatedTimeTenths;
    uint16 CreatedTime;
    uint16 CreatedDate;
    uint16 AccessedDate;
    uint16 FirstClusterHigh;
    uint16 ModifiedTime;
    uint16 ModifiedDate;
    uint16 FirstClusterLow;
    uint32 Size;
} __attribute__((packed)) FAT_dirent;

typedef struct fat_file
{
	BOOL is_dir;
	uint32 position;
	uint32 size;
} fat_file;

void fat_init();
char* fat_read_file(char* fpath);
uint32 fat_path_to_cluster(const char* path);
void fat_dir_ls(uint32 cluster);
#endif
