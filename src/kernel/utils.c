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

uint8* memory_index [MEMORY_INDEX_BASE_SIZE];

void* malloc(uint32 size)
{
  uint32 i = 0;
  while (true)
  {
    // search
    while (memory_index[i] != MEMORY_EMPTY) { i += 2; }
    uint8* last_page_end = memory_index[i - 1];
    uint32 next_page_start_id = i;
    while (memory_index[next_page_start_id] == MEMORY_EMPTY) { ++next_page_start_id; }
    // verify
    if (memory_index[next_page_start_id] - last_page_end > size)
    {
      if (i >= MEMORY_INDEX_BASE_SIZE)
      {
        panic("");
      }
      if (size + last_page_end + 1 >= KERNEL_MEMORY_OFFSET_END)
      {
        panic();
      }
      // allocate
      memory_index[i] = last_page_end + 1;
      memory_index[i + 1] = memory_index[i] + size;
      return (void*) memory_index[i];
    }
  }
}

void memset(uint8 *dest, uint8 val, uint32 len) {
    uint8 *temp = (uint8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void memcpy(char *source, char *dest, int nuint8s) {
    int i;
    for (i = 0; i < nuint8s; i++) {
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

char * strncpy(char* dest, const char *src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i]; i++) {
        dest[i] = src[i];
    }

    if (i < n)
        dest[i] = 0;

    return dest;
}

uint32 free_mem_addr = 0x10000;

uint32 kmalloc(uint32 size, int align, uint32 *phys_addr) {

    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }

    if (phys_addr) *phys_addr = free_mem_addr;

    uint32 ret = free_mem_addr;
    free_mem_addr += size; 
    return ret;
}


void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void hex_to_ascii(int n, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    int32 tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

void page(){
  uint32 phys_addr;
  uint32 page = kmalloc(1000, 1, &phys_addr);
  char page_str[16] = "";
  hex_to_ascii(page, page_str);
  char phys_str[16] = "";
  hex_to_ascii(phys_addr, phys_str);
  print_string("Page: ");
  print_string(page_str);
  print_string(", physical address: ");
  print_string(phys_str);
  print_string("\n");
}

void kfree(void *ptr) {
    mem_header_t *header = (mem_header_t *) (ptr - sizeof(mem_header_t));
    header->flags |= 1; 
}

uint32 memlen(char *s) { // Just strlen() with a different name lol
    uint32 i = 0;
    while (s[i++]);
    return i;
}


