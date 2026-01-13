#include "minirisc.h"
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <program.bin>\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open program file");
    return 1;
  }

  // Create platform (Memory + Bus)
  platform_t *platform = platform_new();

  // Load program into memory at 0x80000000
  // We load it directly into the memory buffer.
  // RAM starts at 0x80000000, which corresponds to offset 0 in our buffer.
  size_t bytes_read = fread(platform->memory, 1, RAM_SIZE, file);
  printf("Loaded %zu bytes from %s into memory.\n", bytes_read, filename);
  fclose(file);

  // Create CPU (starts at 0x80000000)
  minirisc_t *cpu = minirisc_new(0x80000000, platform);

  printf("Starting emulation...\n");
  minirisc_run(cpu);
  printf("Emulation finished.\n");

  // Cleanup
  minirisc_free(cpu);
  platform_free(platform);

  return 0;
}
