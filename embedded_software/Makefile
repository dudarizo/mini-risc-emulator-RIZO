TARGET = esw
BUILD = build

# Toolchain configuration
# Using local compiler extracted in project root
TC_PATH = ../gcc_MINIRISC/bin/
TC_PREFIX = $(TC_PATH)riscv32-MINIRISC-elf-

CC = $(TC_PREFIX)gcc
LD = $(CC)
OBJCOPY = $(TC_PREFIX)objcopy
OBJDUMP = $(TC_PREFIX)objdump
SIZE = $(TC_PREFIX)size

CFLAGS = -march=rv32i -mabi=ilp32
CFLAGS += -W -Wall
CFLAGS += -O2

LDFLAGS = -nostartfiles
LDFLAGS += -Wl,-Ttext=0x80000000

SRCS = $(wildcard *.s)
OBJS = $(addprefix $(BUILD)/, $(SRCS:.s=.o))
DEPS = $(OBJS:.o=.d)

all: $(BUILD)/$(TARGET).bin

$(BUILD)/%.o: %.s
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF "$(@:%.o=%.d)"

$(BUILD)/$(TARGET).elf: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)
	@echo "Size:"
	# $(SIZE) $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/$(TARGET).lss: $(BUILD)/$(TARGET).elf
	$(OBJDUMP) -h -D $< > $@

lss: $(BUILD)/$(TARGET).lss
	less $<

clean:
	@rm -rf $(BUILD)

-include $(DEPS)
