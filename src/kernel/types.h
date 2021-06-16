/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#ifndef TYPES_H
#define TYPES_H

#define NULL 0

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long int uint64;

typedef char int8;
typedef short int16;
typedef int int32;

typedef enum {
    false = 0,
    true = 1
} bool;


typedef long ssize_t;
typedef unsigned long size_t;
typedef long int off_t;

typedef char* string;
#define global_vec(name) \
    typeof(name) name = { 0 }; 

#endif