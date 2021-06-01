#include "utils.h"

uint32 strlen(const char* str)
{
  uint32 length = 0;
  while(str[length])
    length++;
  return length;
}

uint32 digit_count(int num)
{
  uint32 count = 0;
  if(num == 0)
    return 1;
  while(num > 0){
    count++;
    num = num/10;
  }
  return count;
}

void itoa(int num, char *number)
{
  int dgcount = digit_count(num);
  int index = dgcount - 1;
  char x;
  if(num == 0 && dgcount == 1){
    number[0] = '0';
    number[1] = '\0';
  }else{
    while(num != 0){
      x = num % 10;
      number[index] = x + '0';
      index--;
      num = num / 10;
    }
    number[dgcount] = '\0';
  }
}


void * malloc(int nbytes)
{
  char variable[nbytes];
  return &variable;
}

void memset(uint8 *dest, uint8 val, uint32 len) {
    uint8 *temp = (uint8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void memcpy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);             
    }
}

void int_to_ascii(int n, char str[]) {          
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';         
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    /* TODO: implement "reverse" */
    return str;
}

string int_to_string(int n)
{
  string ch = malloc(50);
  int_to_ascii(n,ch);
  int len = strlen(ch);
  int i = 0;
  int j = len - 1;
  while(i<(len/2 + len%2))
  {
    char tmp = ch[i];
    ch[i] = ch[j];
    ch[j] = tmp;
    i++;
    j--;
  }
  return ch;
}

int str_to_int(string ch)
{
  int n = 0;
  int p = 1;
  int strlength = strlen(ch);
  int i;
  for (i = strlength-1;i>=0;i--)
  {
    n += ((int)(ch[i] - '0')) * p;
    p *= 10;
  }
  return n;
}

uint8 strcmp(string ch1,string ch2)                     
{
        uint8 result = 1;
        uint8 size = strlen(ch1);
        if(size != strlen(ch2)) result =0;
        else 
        {
        uint8 i = 0;
        for(i;i<=size;i++)
        {
                if(ch1[i] != ch2[i]) result = 0;
        }
        }
        return result;
}

int memcmp(void *s1, void *s2, int len)
{
    uint8 *p = s1;
    uint8 *q = s2;
    int charCompareStatus = 0;
    //If both pointer pointing same memory block
    if (s1 == s2)
    {
        return charCompareStatus;
    }
    while (len > 0)
    {
        if (*p != *q)
        {
            //compare the mismatching character
            charCompareStatus = (*p >*q)?1:-1;
            break;
        }
        len--;
        p++;
        q++;
    }
    return charCompareStatus;
}

unsigned long next = 1;

int rand(int RAND_MAX){
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % RAND_MAX+1; 
}

extern uint32 kernel_end;
uint32 kernel_end_address = (uint32) &kernel_end;

uint32 total_memory; // in KB

uint32* frame_table;

#define FRAME_SIZE 0x1000
#define FRAME_COUNT      (total_memory / 4)
#define FRAME_TABLE_SIZE (FRAME_COUNT / 32)

void mmap_init(uint32 size) {
  total_memory = size;
  frame_table = malloc(FRAME_TABLE_SIZE);
}

void mmap_init_finalize() {
  // TODO: make this more efficient
  //memset(frame_table, 0xFF, (kernel_end_address / FRAME_SIZE) / 8);
  uint32 i;

  for (i = 0; i < kernel_end_address; i += FRAME_SIZE) {
    mmap_address_set_used(i);
  }
}

void mmap_address_set_free(uint32 address) {
  frame_table[(address / FRAME_SIZE) / 32] &=
    ~ (1 << ((address / FRAME_SIZE) % 32));
}

void mmap_address_set_used(uint32 address) {
  frame_table[(address / FRAME_SIZE) / 32] |=
    (1 << ((address / FRAME_SIZE) % 32));
}

string strchr(const char* str, int c) {
  while (*str != c) {
    if (*str == '\0') {
      return NULL;
    }
    str++;
  }

  return (char*) str;
}

char * strcpy (char *dest, const char *src)
{
    do
        {
            *dest++ = *src++;
        }
    while (*src != 0);
}