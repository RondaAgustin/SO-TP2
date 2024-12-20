#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <registers.h>

#define ITOA_BUFF_MAX_SIZE 100

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpu_vendor(char *result);

void uint64_to_hex_string(uint64_t num, char *buffer, uint16_t buffer_size);

void outb(uint8_t dir, uint8_t val);

uint8_t inb(uint8_t dir);

uint32_t my_strlen(char* s);

uint64_t log2(uint64_t n);

char* my_itoa(uint64_t num, char* dest, uint32_t dest_max_len);

void my_strcpy(char* dest, const char* src);

uint32_t my_strcmp(const char* s1, const char* s2);

#endif