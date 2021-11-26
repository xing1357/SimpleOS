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
         
}

void dump_fs_info(){
    printf("\nNum of Inodes: %x", sb.inode_num);
    printf("\n Num of Blocks: %x", sb.block_num);
    printf("\n Num of Reserved Blocks: %x", sb.reserved_blocks);
    printf("\n Unallocated Blocks: %x", sb.unalloc_blocks);
    printf("\n Unallocated Inodes: %x", sb.unalloc_inodes);
    printf("\n Superblock Address: %x", sb.sb_block_num);
}