/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "tar.h"

uint32 getsize(const char *in)
{
 
    uint32 size = 0;
    uint32 j;
    uint32 count = 1;
 
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
 
}

struct tar_header *headers[32];

uint32 parse(uint32 address)
{
 
    uint32 i;
 
    for (i = 0; ; i++)
    {
 
        struct tar_header *header = (struct tar_header *)address;
 
        if (header->name[0] == '\0')
            break;
 
        uint32 size = getsize(header->size);
 
        headers[i] = header;
 
        address += ((size / 512) + 1) * 512;
 
        if (size % 512)
            address += 512;
 
    }
 
    return i;
 
}