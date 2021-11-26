#ifndef TYPES_H
#define TYPES_H

#define NULL 0

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char sint8;
typedef signed short sint16;
typedef signed int sint32;
typedef uint8 byte;
typedef uint16 word;
typedef uint32 dword;
typedef unsigned long long uint64;
typedef long long int64;
typedef int64 qword;
typedef uint32 size_t; 
typedef uint32 fd_t;
typedef enum {
    FALSE,
    TRUE
} BOOL;

#endif

