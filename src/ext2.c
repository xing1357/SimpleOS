#include "ext2.h"
#include "console.h"
#include "hd.h"
#include "string.h"

void read_superblock(){
    uint32* superblock = malloc(sizeof(*superblock));

    read_sectors_ATA_PIO(superblock, 0x0+2, 1);
    
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
    uint32 index = (inode - 1) / sb.inodes_per_group;
    return index;
}

ext2_bgdt parse_bgdt(uint32 block_group) {
	ext2_bgdt bgdt; 
	uint32 blk_group_start_block = block_group * sb.blocks_per_group + 1;
	uint32 bgdt_start = blk_group_start_block + 1;
	uint32 *bgdt_tmp = malloc(sizeof(bgdt));
	read_sectors_ATA_PIO(bgdt_tmp, 0x0 + bgdt_start + 1, 1);
	bgdt.blk_bmap = bgdt_tmp[0];
	bgdt.inode_bmap = bgdt_tmp[1];
	bgdt.inode_table_start = bgdt_tmp[2];
	bgdt.unalloc_blocks = bgdt_tmp[3];
	bgdt.unalloc_inodes = bgdt_tmp[4];
	bgdt.num_of_dirs = bgdt_tmp[5];
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
    uint32 root_inode_blk_group = determine_blk_group(2);

}
