#ifndef EXT2_H
#define EXT2_H
#include "types.h"

typedef struct ext2_superblock {
    uint32 inode_num;
    uint32 block_num;
    uint32 reserved_blocks;
    uint32 unalloc_blocks;
    uint32 unalloc_inodes;
    uint32 sb_block_num;
    uint32 block_size;
    uint32 fragment_size;
    uint32 inodes_per_group;
}ext2_superblock;

ext2_superblock sb;

void read_superblock();
void dump_fs_info();

#endif