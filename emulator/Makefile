TARGET = emulator
BUILD = build
SRC = $(wildcard *.c)
OBJ = $(addprefix $(BUILD)/, $(SRC:.c=.o))
DEPS = $(OBJ:.o=.d)
CFLAGS = -W -Wall
CFLAGS += -O0 -g
LDFLAGS =

all: $(BUILD)/$(TARGET)

.PHONY: clean exec gdb

-include $(DEPS)

$(BUILD)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

exec: $(BUILD)/$(TARGET)
	./$(BUILD)/$(TARGET)

gdb: $(BUILD)/$(TARGET)
	gdb --args ./$(BUILD)/$(TARGET)

clean:
	@rm -rvf $(BUILD)

$(BUILD)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -MF "$(@:%.o=%.d)" -c $< -o $@
