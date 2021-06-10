#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "drivers/screen/screen.h"
#include "panic.h"


typedef struct __attribute__((packed)) {
    uint16 size : 12;
    uint8 flags : 4;
} mem_header_t;

#define KERNEL_MEMORY_OFFSET_START 0xffffff
#define KERNEL_MEMORY_OFFSET_END	0xffffffff
#define MEMORY_EMPTY 0x0
#define MEMORY_INDEX_BASE_SIZE 10000

uint32 strlen(const char* str);
uint32 digit_count(int num);
void itoa(int num, char *number);
void* malloc(uint32 size);
void memset(uint8 *dest, uint8 val, uint32 len);
void memcpy(char *source, char *dest, int nbytes);
string int_to_string(int n);
int str_to_int(string ch);
void int_to_ascii(int n, char str[]);
uint8 strcmp(string ch1,string ch2);
int memcmp(void *s1, void *s2, int len);
int rand(int RAND_MAX);
string strchr(const char* str, int c);
char * strcpy (char *dest, const char *src);
char * strncpy(char* dest, const char *src, size_t n);
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type)
uint32 kmalloc(uint32 size, int align, uint32 *phys_addr);
void page();
void kfree(void *ptr);
uint32 memlen(char *s);
#endif