#include "fat32.h"
#include "console.h"
#include "ide.h"
#include "string.h"
#include "kheap.h"
#include "panic.h"

#define DRIVE 0
#define FAT_CACHE_SIZE 5

//Defines for Dirent parsing
#define OFFSET_ATTRIB 0x0B
#define OFFSET_CLUS_HI 0x14
#define OFFSET_CLUS_LOW 0x1A
#define FLAG_LFN 0x0F

//Global Variables
uint32 g_bytes_per_sector;
uint32 g_DataSectionLBA;
uint8 g_FAT_Cache[FAT_CACHE_SIZE * 512];
fat_bpb* g_fat_bpb;
uint8* g_root_buf;
const char LFN_idxs[] = {1,3,5,7,9,14,16,18,20,22,24,28,30};

uint32 FAT_ClusterToLba(uint32 cluster)
{
    return g_DataSectionLBA + (cluster - 2) * g_fat_bpb->sectors_per_cluster;
}

void FAT_readfat(uint32 lba_index){
	ide_read_sectors(DRIVE, FAT_CACHE_SIZE, g_fat_bpb->reserved_sector_count + lba_index, g_FAT_Cache);
}

fat_bpb* fat_read_bpb(){
	uint32* bpb = kmalloc(sizeof(fat_bpb));
	ide_read_sectors(DRIVE, 1, 6, bpb);
	return bpb;
}

uint8* load_cluster(int clu){
	uint8* cluster_buf = kmalloc(sizeof(*cluster_buf));
	uint32 LBA = FAT_ClusterToLba(clu);
	ide_read_sectors(DRIVE, g_fat_bpb->sectors_per_cluster, LBA, (uint8*)cluster_buf);
	return cluster_buf;
}

uint32 fat_get_next_clu(uint32 curr_cluster){
	uint32 FAT_cache_position;
	uint32 fat_index = curr_cluster * 4;
	uint32 fat_index_sector = fat_index / 512;
	FAT_readfat(fat_index_sector);
	for(int i = 0; i < 512; i++){
		printf("|%x",g_FAT_Cache[i]);
	}
	FAT_cache_position = fat_index_sector;
	fat_index -= FAT_cache_position * 512;
	uint32 next_cluster;
	next_cluster = *(uint32*)(g_FAT_Cache + fat_index);
	if(next_cluster >= 0x0FFFFFF8){
		return -1; // We have reached the end of the cluster chain
	}
	else {
		return next_cluster;
	}
}

void fat_dir_ls(uint32 cluster){
	uint8* cluster_buf = kmalloc(sizeof(*cluster_buf));
	uint32 cluster_index = 0;
	uint32 record_offset = 0;
	uint32 LFN_index = 0;
	char LFN[105]; //Buffer for long filenames
	char SFN[12]; //Buffer for short filenames
	//set both to 0
	memset(LFN, 0, 105);
	memset(SFN, 0, 12);
	cluster_buf = load_cluster(cluster);

	while(TRUE){
		if(cluster_buf[record_offset] == 0){
			printf("\nNo more");
			break;//No more entries in this cluster
		}
		if(cluster_buf[record_offset] != 0xE5){
			if(cluster_buf[record_offset + OFFSET_ATTRIB] & FLAG_LFN  == FLAG_LFN){// Is it a long filename entry?
				if((cluster_buf[record_offset] & 0xF0) == 0x40){
					LFN_index = cluster_buf[record_offset] & 0x0F;
				}
				if((cluster_buf[record_offset] & 0x0F) == LFN_index){
					LFN_index--;
				}
				else {
					printf("\nError");
				}
				int byte_pos;
				char LFN_char;
				for(int i = 0; i < 13; i++){
					byte_pos = record_offset + LFN_idxs[i];
					LFN_char = cluster_buf[byte_pos + 1] == 0x00 ? cluster_buf[byte_pos] : 0;
					LFN[LFN_index * 13 + i] = LFN_char;
				}
			}

			else { //It is a regular 8.3 entry
				memcpy(SFN, cluster_buf+record_offset, 11);
				printf("\n%s", LFN[0] == 0 ? SFN : LFN);
				memset(LFN, 0, 105);
			}			
		}		
		//printf("|%x", cluster_buf[record_offset]);
		record_offset += 32;
		if(record_offset == g_fat_bpb->sectors_per_cluster * g_fat_bpb->bytes_per_sector){
			uint32 next_cluster = fat_get_next_clu(cluster);
			if(next_cluster == -1){
				break;
			}
			else {
				cluster_buf = load_cluster(next_cluster);
			}
		}
	}
}

uint32 fat_find_in_dir(char* fname, int cluster){
	uint8* cluster_buf = load_cluster(cluster);
	uint32 cluster_index = 0;
	uint32 record_offset = 0;
	uint32 LFN_index = 0;
	char LFN[105]; //Buffer for long filenames
	char SFN[12]; //Buffer for short filenames
	//set both to 0
	memset(LFN, 0, 105);
	memset(SFN, 0, 12);
	while(TRUE){
		if(cluster_buf[record_offset] == 0){
			break;//No more entries in this cluster
		}
		if(cluster_buf[record_offset] != 0xE5){
			if(cluster_buf[record_offset + OFFSET_ATTRIB] &FLAG_LFN  == FLAG_LFN){// Is it a long filename entry?
				if((cluster_buf[record_offset] & 0xF0) == 0x40){
					LFN_index = cluster_buf[record_offset] & 0x0F;
				}
				if((cluster_buf[record_offset] & 0x0F) == LFN_index){
					LFN_index--;
				}
				else {
					printf("\nError");
				}
				int byte_pos;
				char LFN_char;
				for(int i = 0; i < 13; i++){
					byte_pos = record_offset + LFN_idxs[i];
					LFN_char = cluster_buf[byte_pos + 1] == 0x00 ? cluster_buf[byte_pos] : 0;
					LFN[LFN_index * 13 + i] = LFN_char;
				}
			}

			else { //It is a regular 8.3 entry
				memcpy(SFN, cluster_buf+record_offset, 11);
				if(strcmp(LFN, fname) == 0 || (LFN[0] == 0 && strcmp(SFN, fname) == 0)){
					uint32 ret_cluster = (*(uint16*)(cluster_buf + (record_offset + OFFSET_CLUS_HI))) << 16;
					ret_cluster |= (*(uint16*)(cluster_buf+ (record_offset + OFFSET_CLUS_LOW)));
					return ret_cluster;
					break;
				}
				printf("\n%s", LFN[0] == 0 ? SFN : LFN);
				memset(LFN, 0, 105);
			}			
		}		
		//printf("|%x", cluster_buf[record_offset]);
		record_offset += 32;
		if(record_offset == g_fat_bpb->sectors_per_cluster * g_fat_bpb->bytes_per_sector){
			uint32 next_cluster = fat_get_next_clu(cluster);
			if(next_cluster == -1){
				break;
			}
			else {
				cluster_buf = load_cluster(next_cluster);
			}
		}
	}
	return -1;		
}

char* fat_get_filedata(uint32 cluster){
	char* file_buf = load_cluster(cluster);
	return file_buf;
}

uint32 fat_path_to_cluster(const char* path){
	uint32 last_cluster = 2;
	int start;
	int end;
	int i = 0;
	BOOL found_tk = FALSE;
	if(!strcmp(path, "/"))
		return 2;

	while(path[i]){
		if(path[i] == '/' && found_tk == FALSE){
			//printf("\n(1)Current Path: %s", path);
			start = i;
			found_tk = TRUE;
			//printf("\n(1)After path: %s", path);
		}

		else if(path[i] == '/' && found_tk == TRUE){
			//printf("\n(2)Before path: %s", path);
			end = i;
			char* lookup_name = kcalloc(end-start, 1);
			//printf("\nAfter path: %s", path);
			//printf("\nStart: %d End: %d", start, end);
			int y = 0;
			for(int x = start + 1; x < end; x++){
				//printf("\nCurrent Path: %s", path);
				lookup_name[y] = path[x];
				//printf("|%x", lookup_name[y]);
				y++;
			}
			start = end;
			//found_tk = FALSE;
			//printf("\nCurrent Path: %s", path);
			//printf("\nName: %s", lookup_name);
			last_cluster = fat_find_in_dir(lookup_name, last_cluster);
		}

		else if(i == strlen(path) - 1 && found_tk == TRUE){
			//printf("\nI: %d Strlen: %d", i, strlen(path));
			end = i;
			//printf("nStart: %d End: %d", start, end);
			char* lookup_name = kcalloc(end - start + 1, 1);
			int y = 0;
			for(int x = start + 1; x <= end; x++){
				lookup_name[y] = path[x];
				//printf("|%c", lookup_name[y]);
				y++;
			}
			start = end - 1;
			//printf("\nName: %s", lookup_name);	
			last_cluster = fat_find_in_dir(lookup_name, last_cluster);
		}
		i++;
	}
	return last_cluster;
}

char* fat_read_file(char* fpath){
	uint32 file_clu = fat_path_to_cluster(fpath);
	char* file_buf = fat_get_filedata(file_clu);
	return file_buf;
}

void fat_init(){
	g_fat_bpb = fat_read_bpb();
	uint32 root_dir_lba;
	g_bytes_per_sector = g_fat_bpb->bytes_per_sector;
	g_DataSectionLBA = g_fat_bpb->reserved_sector_count + g_fat_bpb->table_size_32 * g_fat_bpb->table_count;
	root_dir_lba = FAT_ClusterToLba(g_fat_bpb->root_cluster);
	printf("\nSectors Per Cluster: %d", g_fat_bpb->sectors_per_cluster);
	printf("\nSectors Per FAT: %d", g_fat_bpb->table_size_32);
	printf("\nTable Count: %d", g_fat_bpb->table_count);
	printf("\nData LBA: %d", g_DataSectionLBA);
	printf("\nRoot Cluster: %d", g_fat_bpb->root_cluster);
	printf("\nRoot LBA: %d\n", root_dir_lba);
	ide_read_sectors(DRIVE, 1, root_dir_lba, g_root_buf);
	char* file_buf = fat_read_file("/test/subdir/hello.txt");
	printf("\nFile buf: %s\n", file_buf);
}
