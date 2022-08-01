#include "ext2.h"
#include "console.h"
#include "ide.h"
#include "string.h"
#include "kheap.h"

#define DRIVE 0 // We are using drive 0, the harddisk

void read_superblock(){
    uint32* superblock = kmalloc(sizeof(*superblock));

    //read_sectors_ATA_PIO(superblock, 2, 1);
    ide_read_sectors(DRIVE, 1, 2, superblock);
    sb.inode_num = superblock[0];
    sb.block_num = superblock[1];
    sb.reserved_blocks = superblock[2];
    sb.unalloc_blocks = superblock[3]; 
    sb.unalloc_inodes = superblock[4];  
   	sb.sb_block_num = superblock[5];
    sb.block_size = 1024 << superblock[6];
    sb.fragment_size = 1024 << superblock[7];
    sb.blocks_per_group = superblock[8];
    sb.fragments_per_group = superblock[9];
    sb.inodes_per_group = superblock[10];         
}

static int lba_to_ext2_block(int block_num_LBA)
{
	 int ext2block = block_num_LBA - 1;
	 return ext2block;
}

uint32 determine_blk_group(uint32 inode){
    uint32 block_group = (inode - 1) / sb.inodes_per_group;
    return block_group;
}

uint32 get_inode_index(uint32 inode){
    uint32 index = (inode - 1) % sb.inodes_per_group;
    return index;
}

ext2_bgdt parse_bgdt(uint32 block_group) {
	ext2_bgdt bgdt; 
	uint32 blk_group_start_block = (block_group - 1) * sb.blocks_per_group + 1;
	uint32 bgdt_start = blk_group_start_block + 2;
	uint32 *bgdt_tmp = kmalloc(sizeof(bgdt));
	ide_read_sectors(DRIVE, 1, bgdt_start + 1, bgdt_tmp);
	bgdt.blk_bmap = bgdt_tmp[0];
	bgdt.inode_bmap = bgdt_tmp[1];
	bgdt.inode_table_start = bgdt_tmp[2];
	bgdt.unalloc_blocks = bgdt_tmp[3];
	bgdt.unalloc_inodes = bgdt_tmp[4];
	bgdt.num_of_dirs = bgdt_tmp[5];
	return bgdt;
}

void get_inode_type(uint32 type){
	switch(type) {
			default:
				printf("\nUnknown Inode Type");	
				printf("%x", type);	
				break;
			case INODE_TYPE_FIFO:
				printf("\nInode Type: FIFO");
				break;
			case INODE_TYPE_CHAR_DEV:
				printf("\nInode Type: Character Device");
				break;
			case INODE_TYPE_DIR:
				printf("\nInode Type: Directory");
				break;
			case INODE_TYPE_BLK_DEV:
				printf("\nInode Type: Block Device");
				break;
			case INODE_TYPE_FILE:
				printf("\nInode Type: Regular File");
				break;
			case INODE_TYPE_SYMB_LINK:
				printf("\nInode Type: Symbolic Link");
				break;
			case INODE_TYPE_UNIX_SOCKET:
				printf("\nInode Type: Unix Socket");
				break;	
		}
}
ext2_inode read_inode(uint32 inode) {
	uint32 group = determine_blk_group(inode) + 1;
	uint32 index = get_inode_index(inode);
	ext2_bgdt inode_bgdt;
	inode_bgdt = parse_bgdt(group);
	uint32 inode_table_start = inode_bgdt.inode_table_start;
	printf("\nReading Inode: %d", inode);
	printf("\nInode Table Start: %d", inode_table_start);
	uint32 containing_block = (index * INODE_SIZE) / sb.block_size;
	uint32* inode_tmp = malloc(sizeof(inode_tmp));
	ide_read_sectors(DRIVE, 2, 2 * (inode_table_start + containing_block), inode_tmp);
	ext2_inode inode_info;
	uint32 index_in_block = inode % 4;
	printf("\nIndex: %d", index_in_block);
	// For debugging; prints out the raw inode contents
	if(index_in_block == 1) {
		for(int i = 0; i <= 63; i++){
			printf("%x", inode_tmp[i]);
			printf("|");		
		}
		inode_info.type_perm = inode_tmp[0];
		inode_info.type = (inode_info.type_perm & 0xF000) >> 12;
		inode_info.perm = (inode_info.type_perm & 0x0FFF) >> 0;
		inode_info.user_id = (uint32)inode_tmp[1];
		inode_info.last_access_time = (uint32)inode_tmp[2];
		inode_info.creation_time = (uint32)inode_tmp[3];
		inode_info.modification_time = (uint32)inode_tmp[4];
		inode_info.deletion_time = (uint32)inode_tmp[5];
		inode_info.direct_block_pointers[0] = (uint32)inode_tmp[10];
		inode_info.direct_block_pointers[1] = (uint32)inode_tmp[11];
		inode_info.direct_block_pointers[2] = (uint32)inode_tmp[12];
		inode_info.direct_block_pointers[3] = (uint32)inode_tmp[13];
		inode_info.direct_block_pointers[4] = (uint32)inode_tmp[14];
		inode_info.direct_block_pointers[5] = (uint32)inode_tmp[15];
		inode_info.direct_block_pointers[6] = (uint32)inode_tmp[16];
		inode_info.direct_block_pointers[7] = (uint32)inode_tmp[17];
		inode_info.direct_block_pointers[8] = (uint32)inode_tmp[18];
		inode_info.direct_block_pointers[9] = (uint32)inode_tmp[19];
		inode_info.direct_block_pointers[10] = (uint32)inode_tmp[20];
		inode_info.direct_block_pointers[11] = (uint32)inode_tmp[21];
		get_inode_type(inode_info.type);	
		printf("\nHex Permissions: %x", inode_info.perm);
		printf("\nUser ID: %d", inode_info.user_id);
		printf("\nData Blocks: ");
		for(int i = 0; i <= 11; i++) {
			printf(" %d",inode_info.direct_block_pointers[i]);
		}
		
	}

	else if(index_in_block == 2) {
		/*for(int i = 64; i <= 127; i++ ){
			printf("%x",inode_tmp[i]);
			printf("|");			
		}*/
		inode_info.type_perm = inode_tmp[64];
		inode_info.type = (inode_info.type_perm & 0xF000) >> 12;
		inode_info.perm = (inode_info.type_perm & 0x0FFF) >> 0;
		inode_info.user_id = (uint32)inode_tmp[65];
		inode_info.last_access_time = (uint32)inode_tmp[66];
		inode_info.creation_time = (uint32)inode_tmp[67];
		inode_info.modification_time = (uint32)inode_tmp[68];
		inode_info.deletion_time = (uint32)inode_tmp[69];
		inode_info.direct_block_pointers[0] = (uint32)inode_tmp[74];
		inode_info.direct_block_pointers[1] = (uint32)inode_tmp[75];
		inode_info.direct_block_pointers[2] = (uint32)inode_tmp[76];
		inode_info.direct_block_pointers[3] = (uint32)inode_tmp[77];
		inode_info.direct_block_pointers[4] = (uint32)inode_tmp[78];
		inode_info.direct_block_pointers[5] = (uint32)inode_tmp[79];
		inode_info.direct_block_pointers[6] = (uint32)inode_tmp[80];
		inode_info.direct_block_pointers[7] = (uint32)inode_tmp[81];
		inode_info.direct_block_pointers[8] = (uint32)inode_tmp[82];
		inode_info.direct_block_pointers[9] = (uint32)inode_tmp[83];
		inode_info.direct_block_pointers[10] = (uint32)inode_tmp[84];
		inode_info.direct_block_pointers[11] = (uint32)inode_tmp[85];
		get_inode_type(inode_info.type);
		printf("\nHex Permissions: %x", inode_info.perm);
		printf("\nUser ID: %d", inode_info.user_id);
		printf("\nData Blocks: ");
		for(int i = 0; i <= 11; i++) {
			printf(" %d",inode_info.direct_block_pointers[i]);
		}
	}
	else if(index_in_block == 3) {
		for(int i = 128; i <= 191; i++ ){
			printf("%x",inode_tmp[i]);
			printf("|");			
		}
		inode_info.type_perm = inode_tmp[128];
		inode_info.type = (inode_info.type_perm & 0xF000) >> 12;
		inode_info.perm = (inode_info.type_perm & 0x0FFF) >> 0;
		inode_info.user_id = (uint32)inode_tmp[129];
		inode_info.last_access_time = (uint32)inode_tmp[130];
		inode_info.creation_time = (uint32)inode_tmp[131];
		inode_info.modification_time = (uint32)inode_tmp[132];
		inode_info.deletion_time = (uint32)inode_tmp[133];
		inode_info.direct_block_pointers[0] = (uint32)inode_tmp[138];
		inode_info.direct_block_pointers[1] = (uint32)inode_tmp[139];
		inode_info.direct_block_pointers[2] = (uint32)inode_tmp[140];
		inode_info.direct_block_pointers[3] = (uint32)inode_tmp[141];
		inode_info.direct_block_pointers[4] = (uint32)inode_tmp[142];
		inode_info.direct_block_pointers[5] = (uint32)inode_tmp[143];
		inode_info.direct_block_pointers[6] = (uint32)inode_tmp[144];
		inode_info.direct_block_pointers[7] = (uint32)inode_tmp[145];
		inode_info.direct_block_pointers[8] = (uint32)inode_tmp[146];
		inode_info.direct_block_pointers[9] = (uint32)inode_tmp[147];
		inode_info.direct_block_pointers[10] = (uint32)inode_tmp[148];
		inode_info.direct_block_pointers[11] = (uint32)inode_tmp[149];
		get_inode_type(inode_info.type);
		printf("\nHex Permissions: %x", inode_info.perm);
		printf("\nUser ID: %d", inode_info.user_id);
		printf("\nData Blocks: ");
		for(int i; i <= 11; i++) {
			printf(" %d",inode_info.direct_block_pointers[i]);
		}
	}
	else if(index_in_block == 4) {
		/*for(int i = 64; i <= 127; i++ ){
			printf("%x",inode_tmp[i]);
			printf("|");			
		}*/
		inode_info.type_perm = inode_tmp[192];
		inode_info.type = (inode_info.type_perm & 0xF000) >> 12;
		inode_info.perm = (inode_info.type_perm & 0x0FFF) >> 0;
		inode_info.user_id = (uint32)inode_tmp[193];
		inode_info.last_access_time = (uint32)inode_tmp[194];
		inode_info.creation_time = (uint32)inode_tmp[195];
		inode_info.modification_time = (uint32)inode_tmp[196];
		inode_info.deletion_time = (uint32)inode_tmp[197];
		inode_info.direct_block_pointers[0] = (uint32)inode_tmp[202];
		inode_info.direct_block_pointers[1] = (uint32)inode_tmp[203];
		inode_info.direct_block_pointers[2] = (uint32)inode_tmp[204];
		inode_info.direct_block_pointers[3] = (uint32)inode_tmp[205];
		inode_info.direct_block_pointers[4] = (uint32)inode_tmp[206];
		inode_info.direct_block_pointers[5] = (uint32)inode_tmp[207];
		inode_info.direct_block_pointers[6] = (uint32)inode_tmp[208];
		inode_info.direct_block_pointers[7] = (uint32)inode_tmp[209];
		inode_info.direct_block_pointers[8] = (uint32)inode_tmp[210];
		inode_info.direct_block_pointers[9] = (uint32)inode_tmp[211];
		inode_info.direct_block_pointers[10] = (uint32)inode_tmp[212];
		inode_info.direct_block_pointers[11] = (uint32)inode_tmp[213];
		get_inode_type(inode_info.type);
		printf("\nHex Permissions: %x", inode_info.perm);
		printf("\nUser ID: %d", inode_info.user_id);
		printf("\nData Blocks: ");
		for(int i; i <= 11; i++) {
			printf(" %d",inode_info.direct_block_pointers[i]);
		}
	}
	else {
		printf("Inode type given not uint32\n");
	}

	return inode_info;
}

ext2_dirent ext2_read_dirent(uint32* data, uint32 index){
	ext2_dirent dirent;
	dirent.inode = data[index] & 0xFF;
	//printf("\nInode: %d", dirent.inode);
	dirent.dirent_size = data[index + 1] & 0xFF;
	//printf("\nDirent Size: %d", dirent.dirent_size);
	dirent.name_len = (data[index + 1] & 0x0FF000) >> 16;
	//printf("\nName Size: %d", (uint32)dirent.name_len);
	getdata(dirent.name, dirent.name_len, index + 2, data);
	char * tmp = kcalloc(dirent.name_len + 1, 1);
	memcpy(tmp, dirent.name, dirent.name_len);
	dirent.name = tmp;
	return dirent;
}

void ext2_read_dir(uint32 inode_num){
	ext2_inode inode = read_inode(inode_num);
	uint32* dir = malloc(sizeof(*dir));
	uint32 datablock0 = inode.direct_block_pointers[0];
	ide_read_sectors(DRIVE, 6, 2 * datablock0, dir);
	int curr = 0;
	int next;
	while(1){
		ext2_dirent curr_dirent;
		curr_dirent = ext2_read_dirent(dir, curr);
		if(curr_dirent.inode == 0){
			break;
		}
		//printf("\nInode: %d ", curr_dirent.inode);
		//printf("\nDirent Size: %d", curr_dirent.dirent_size);
		next = curr_dirent.dirent_size / 4;
		//printf("\nName Size: %d", curr_dirent.name_len);
		printf("%s", curr_dirent.name, curr_dirent.inode);
		curr = curr + next;
	}
}

void getdata(char* data, uint32 len, uint32 start, uint32* from){
	for(int i = 0; i < len; i++){
		uint32 index = i % 4;		
		if(index == 0)
			data[i] = from[i + start - 3 * (i / 4)] & 0xFF;
		else if(index == 1)
			data[i] = (from[i + start - 1 - 3 * (i / 4)] >> 8) & 0xFF;
		else if(index == 2)
			data[i] = (from[i + start - 2 - 3 * (i / 4)] >> 16) & 0xFF;
		else if(index == 3)
			data[i] = (from[i + start - 3 - 3 * (i / 4)] >> 24) & 0xFF;						
	}
}

void dump_fs_info(){
    printf("\nNum of Inodes: %d", sb.inode_num);
    printf("\n Num of Blocks: %d", sb.block_num);
    printf("\n Num of Reserved Blocks: %d", sb.reserved_blocks);
    printf("\n Unallocated Blocks: %d", sb.unalloc_blocks);
    printf("\n Unallocated Inodes: %d", sb.unalloc_inodes);
    printf("\n Superblock Address: %d", sb.sb_block_num);
    printf("\n Block Size: %d", sb.block_size);
    printf("\n Fragment Size: %d", sb.fragment_size);
	printf("\nListing Root dir: /\n");
	ext2_read_dir(2);
	printf("\nListing /test\n");
	ext2_read_dir(13);
}

