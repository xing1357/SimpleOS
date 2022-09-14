#ifndef STRING_H
#define STRING_H

#include "types.h"
#include "multiboot.h"

uint32 digit_count(int num, int base);

void kfree(void* memory);

void *memset(void *dst, char c, uint32 n);

void *memcpy(void *dst, const void *src, uint32 n);

int memcmp(uint8 *s1, uint8 *s2, uint32 n);

BOOL strncmp(char *a, char *b, uint32 n); 

int strlen(const char *s);

int strcmp(const char *s1, char *s2);

int strcpy(char *dst, const char *src);

void strcat(char *dest, const char *src);

void strncat(char *dest, const char *src, int n);

uint32 strsplit(char* str, char delim);

int isspace(char c);

int isalpha(char c);
char upper(char c);
char lower(char c);

void itoa(char *buf, int base, int d);

void * malloc(int nbytes);
int str_to_int(char* ch);
int toDeci(int hex, int base);

#endif
