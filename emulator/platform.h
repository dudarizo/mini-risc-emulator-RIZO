#ifndef PLATFORM_H
#define PLATFORM_H

#include <stddef.h>
#include <stdint.h>

// Memory constants
#define RAM_SIZE (32 * 1024 * 1024) // 32 MiB
#define RAM_BASE_ADDR 0x80000000
#define RAM_END_ADDR (RAM_BASE_ADDR + RAM_SIZE)

// Peripheral constants
#define CHAROUT_BASE_ADDR 0x10000000
#define CHAROUT_END_ADDR 0x1000000C

typedef struct {
  uint8_t *memory; // 32 MiB buffer
} platform_t;

// Constructor and Destructor
platform_t *platform_new();
void platform_free(platform_t *platform);

// Bus Access Functions
uint32_t platform_read_32(platform_t *platform, uint32_t address);
uint16_t platform_read_16(platform_t *platform, uint32_t address);
uint8_t platform_read_8(platform_t *platform, uint32_t address);

void platform_write_32(platform_t *platform, uint32_t address, uint32_t value);
void platform_write_16(platform_t *platform, uint32_t address, uint16_t value);
void platform_write_8(platform_t *platform, uint32_t address, uint8_t value);

#endif // PLATFORM_H
