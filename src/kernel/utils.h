#ifndef UTILS_H
#define UTILS_H

#include "types.h"

uint32 strlen(const char* str);
uint32 digit_count(int num);
void itoa(int num, char *number);
void * malloc(int nbytes);
void memset(uint8 *dest, uint8 val, uint32 len);
void memcpy(char *source, char *dest, int nbytes);
string int_to_string(int n);
int str_to_int(string ch);
void int_to_ascii(int n, char str[]);
uint8 strcmp(string ch1,string ch2);
int memcmp(void *s1, void *s2, int len);
int rand(int RAND_MAX);
void mmap_init(uint32 size);
void mmap_init_finalize();
void mmap_address_set_free(uint32 address);
void mmap_address_set_used(uint32 address);
string strchr(const char* str, int c);
char * strcpy (char *dest, const char *src);
char * strncpy(char* dest, const char *src, size_t n);

#endif