#include "ext2.h"
#include "console.h"
#include "hd.h"
#include "string.h"


void read_superblock(){
    uint32* superblock = malloc(sizeof(*superblock));

    read_sectors_ATA_PIO(superblock, 0x0+2, 1);
    
    int inode_num = toDeci(superblock[0], 16);
    sb.inode_num = (uint32)inode_num;
    int block_num = toDeci(superblock[1], 16);
    sb.block_num = (uint32)block_num;
    int reserved_blocks = toDeci(superblock[2], 16);
    sb.reserved_blocks = (uint32)reserved_blocks;
    int unalloc_blocks = toDeci(superblock[3], 16);
    sb.unalloc_blocks = (uint32)unalloc_blocks;   
    int unalloc_inodes = toDeci(superblock[4], 16);
    sb.unalloc_inodes = (uint32)unalloc_inodes;   
    int sb_block_num = toDeci(superblock[5], 16);
    sb.sb_block_num = (uint32)sb_block_num;
    int block_size = toDeci(superblock[6], 16);
    sb.block_size = 1024 << (uint32)block_size;
    int fragment_size = toDeci(superblock[7], 16);
    sb.fragment_size = 1024 << (uint32)fragment_size;
    int fragments_per_group = toDeci(superblock[8], 16);
    sb.fragments_per_group = (uint32)fragments_per_group;
    int inodes_per_group = toDeci(superblock[9], 16);
    sb.inodes_per_group = (uint32)inodes_per_group;
         
}

uint32 determine_blk_group(uint32 inode){
    uint32 block_group = (inode - 1) / sb.inodes_per_group;
    return block_group;
}

uint32 get_inode_block(uint32 inode){
    int index = (inode - 1) / sb.inodes_per_group;
    uint32 containing_block = (index * 128) / sb.block_size;
    return containing_block;
}

void dump_fs_info(){
    printf("\nNum of Inodes: %x", sb.inode_num);
    printf("\n Num of Blocks: %x", sb.block_num);
    printf("\n Num of Reserved Blocks: %x", sb.reserved_blocks);
    printf("\n Unallocated Blocks: %x", sb.unalloc_blocks);
    printf("\n Unallocated Inodes: %x", sb.unalloc_inodes);
    printf("\n Superblock Address: %x", sb.sb_block_num);
    printf("\n Block Size: %x", sb.block_size);
    printf("\n Fragment Size: %x", sb.fragment_size);
}