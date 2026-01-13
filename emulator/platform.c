#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

platform_t *platform_new() {
  platform_t *platform = (platform_t *)malloc(sizeof(platform_t));
  if (!platform) {
    perror("Failed to allocate platform struct");
    exit(1);
  }

  platform->memory = (uint8_t *)malloc(RAM_SIZE);
  if (!platform->memory) {
    perror("Failed to allocate RAM");
    free(platform);
    exit(1);
  }
  // Initialize memory to 0
  memset(platform->memory, 0, RAM_SIZE);

  return platform;
}

void platform_free(platform_t *platform) {
  if (platform) {
    if (platform->memory) {
      free(platform->memory);
    }
    free(platform);
  }
}

// Helper to check if address is in RAM
static int is_ram(uint32_t address) {
  return (address >= RAM_BASE_ADDR && address < RAM_END_ADDR);
}

// Helper to check if address is CharOut
static int is_charout(uint32_t address) {
  return (address >= CHAROUT_BASE_ADDR && address < CHAROUT_END_ADDR);
}

uint32_t platform_read_32(platform_t *platform, uint32_t address) {
  if (is_ram(address)) {
    uint32_t offset = address - RAM_BASE_ADDR;
    // Little-endian read
    return (uint32_t)platform->memory[offset] |
           ((uint32_t)platform->memory[offset + 1] << 8) |
           ((uint32_t)platform->memory[offset + 2] << 16) |
           ((uint32_t)platform->memory[offset + 3] << 24);
  } else {
    // Unmapped or write-only peripheral read
    fprintf(stderr, "Read fault at address 0x%08x\n", address);
    return 0;
  }
}

uint16_t platform_read_16(platform_t *platform, uint32_t address) {
  if (is_ram(address)) {
    uint32_t offset = address - RAM_BASE_ADDR;
    return (uint16_t)platform->memory[offset] |
           ((uint16_t)platform->memory[offset + 1] << 8);
  } else {
    fprintf(stderr, "Read fault at address 0x%08x\n", address);
    return 0;
  }
}

uint8_t platform_read_8(platform_t *platform, uint32_t address) {
  if (is_ram(address)) {
    return platform->memory[address - RAM_BASE_ADDR];
  } else {
    fprintf(stderr, "Read fault at address 0x%08x\n", address);
    return 0;
  }
}

void platform_write_32(platform_t *platform, uint32_t address, uint32_t value) {
  if (is_ram(address)) {
    uint32_t offset = address - RAM_BASE_ADDR;
    platform->memory[offset] = value & 0xFF;
    platform->memory[offset + 1] = (value >> 8) & 0xFF;
    platform->memory[offset + 2] = (value >> 16) & 0xFF;
    platform->memory[offset + 3] = (value >> 24) & 0xFF;
  } else if (is_charout(address)) {
    if (address == 0x10000000) {
      // Write char
      printf("%c", (char)value);
    } else if (address == 0x10000004) {
      // Write signed int
      printf("%d", (int)value);
    } else if (address == 0x10000008) {
      // Write unsigned hex
      printf("%x", (unsigned int)value);
    }
    fflush(stdout); // Ensure output is seen immediately
  } else {
    fprintf(stderr, "Write fault at address 0x%08x\n", address);
  }
}

void platform_write_16(platform_t *platform, uint32_t address, uint16_t value) {
  if (is_ram(address)) {
    uint32_t offset = address - RAM_BASE_ADDR;
    platform->memory[offset] = value & 0xFF;
    platform->memory[offset + 1] = (value >> 8) & 0xFF;
  } else {
    // CharOut registers are 32-bit aligned, usually accessed as words, but we
    // can ignore or handle if needed. For now, treat as fault if not RAM.
    fprintf(stderr, "Write fault at address 0x%08x\n", address);
  }
}

void platform_write_8(platform_t *platform, uint32_t address, uint8_t value) {
  if (is_ram(address)) {
    platform->memory[address - RAM_BASE_ADDR] = value;
  } else if (is_charout(address)) {
    if (address == 0x10000000) {
      printf("%c", (char)value);
      fflush(stdout);
    }
  } else {
    fprintf(stderr, "Write fault at address 0x%08x\n", address);
  }
}
