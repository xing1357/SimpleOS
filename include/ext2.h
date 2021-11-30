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
    uint32 fragments_per_group;
    uint32 inodes_per_group;
}ext2_superblock;

typedef struct ext2_bgdt {
    uint32 blk_addr_blk_bitmap;
    uint32 blk_addr_inode_bitmap;
    uint32 inode_table_start;
    uint32 unalloc_blocks;
    uint32 unalloc_inodes;
    uint32 num_of_dirs;
}ext2_bgdt;

ext2_superblock sb;
ext2_bgdt bgdt;

void read_superblock();
void dump_fs_info();

#endif