/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#pragma once

#ifndef FS_H
#define FS_H

#include "../types.h"

#define OK 0
#define FILE_COUNT_MAX_EXCEEDED 1
#define FILE_ALREADY_EXISTS 2
#define FILE_NAME_INVALID 3
#define FILE_NOT_FOUND 1

#define FS_SECTOR_SIZE 512
#define FS_MAX_FILE_COUNT 1000
#define FS_FILE_NAME_BUFFER 512
#define FS_FILE_TAGS_BUFFER 512
#define FS_FILE_NAME_VALID_CHARS "qwertyuiiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM"

void fsinit();
int file_remove(char* name);
int file_make(char* name);
char* file_get_name(int id);
int file_get_id(char* name);
int file_count();
bool file_exists(char* name);
int file_size(char* name);
int file_read(char* filename, char* output);
int file_write(char* filename, char* data, uint32 depth);
int file_writes(char* filename, char* text);
int file_clean(char* filename);
#endif