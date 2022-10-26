#ifndef EXT2_H
#define EXT2_H
#include "types.h"

#define EXT2_SUPER 1

#define INODE_SIZE 256 //Hardcoded here cause im lazy @TODO read the actual superblock field dummy

#define INODE_TYPE_FIFO 1
#define INODE_TYPE_CHAR_DEV 2
#define INODE_TYPE_DIR 0x4
#define INODE_TYPE_BLK_DEV 6
#define INODE_TYPE_FILE 8
#define INODE_TYPE_SYMB_LINK 0xA000
#define INODE_TYPE_UNIX_SOCKET 0xC000

//Should move these defs into a new header file later, but putting this here for now
#define FILE_NOT_FOUND 0

typedef struct ext2_superblock {
    uint32 inodes_count;			// Total # of inodes
	uint32 blocks_count;			// Total # of blocks
	uint32 r_blocks_count;		// # of reserved blocks for superuser
	uint32 free_blocks_count;	
	uint32 free_inodes_count;
	uint32 first_data_block;
	uint32 log_block_size;		// 1024 << Log2 block size  = block size
	uint32 log_frag_size;
	uint32 blocks_per_group;
	uint32 frags_per_group;
	uint32 inodes_per_group;
	uint32 mtime;					// Last mount time, in POSIX time
	uint32 wtime;					// Last write time, in POSIX time
	uint16 mnt_count;				// # of mounts since last check
	uint16 max_mnt_count;			// # of mounts before fsck must be done
	uint16 magic;					// 0xEF53
	uint16 state;
	uint16 errors;
	uint16 minor_rev_level;
	uint32 lastcheck;
	uint32 checkinterval;
	uint32 creator_os;
	uint32 rev_level;
	uint16 def_resuid;
	uint16 def_resgid;
}ext2_superblock;

typedef struct ext2_bgdt {
    uint32 blk_bmap;
    uint32 inode_bmap;
    uint32 inode_table_start;
    uint16 unalloc_blocks;
    uint16 unalloc_inodes;
    uint16 num_of_dirs;
    uint16 pad[7];
}ext2_bgdt;

typedef struct ext2_inode
{
    unsigned short type_perm;
    unsigned short user_id;
    unsigned int size_low;
    unsigned int last_access_time;
    unsigned int creation_time;
    unsigned int modification_time;
    unsigned int deletion_time;
    unsigned short group_id;
    unsigned short hard_links;
    unsigned int sector_usage;
    unsigned int flags;
    unsigned int OS_type;
    uint32 direct_block_pointers[12];
    unsigned int singly_indirect_block_pointer;
    unsigned int doubly_indirect_block_pointer;
    unsigned int triply_indirect_block_pointer;
    unsigned int generation_num;
    unsigned int extended_attribute_block;
    unsigned int size_high_dir_acl;
    unsigned int block_addr_of_fragment;
    unsigned char os_specific[12];
} ext2_inode;

typedef struct ext2_dirent
{
	uint32 inode;
	uint32 dirent_size;
	uint32 name_len;
	uint32 type;
	char* name;
} ext2_dirent;

ext2_superblock* sb;

void read_superblock();
void ext2_init();
char* ext2_read_file(char* fpath);
char** ext2_ls(uint32 inode_num);
uint32 ext2_path_to_inode(char* path);
#endif
