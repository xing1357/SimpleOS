/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#ifndef INITRD_H
#define INITRD_H

#include "../../types.h"
#include "../vfs.h"
#include "../../utils.h"

typedef struct
{
    uint32 nfiles;  /* The number of files in the ramdisk */
} initrd_header_t;

typedef struct
{
    uint8  magic;    /* Magic number; for error checking. */
    char name[64]; /* Filename */
    uint32 offset;   /* Offset in the initrd that the file starts. */
    uint32 length;   /* Length of the file. */
} initrd_file_header_t;

/* Initialises the initial ramdisk. It gets passed the address of the
 * multiboot module, and returns a completed filesystem node.
 */
fs_node_t *initialise_initrd (uint32 location);

#endif