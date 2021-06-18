/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#ifndef TAR_H
#define TAR_H

#include "../types.h"


struct tar_header
{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
};

uint32 getsize(const char *in);
uint32 parse(uint32 address);

#endif