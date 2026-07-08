# Makefile
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS  = -mcpu=cortex-m3 -mthumb
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -O2 -g
CFLAGS += -Wall -Wextra
CFLAGS += -ffunction-sections -fdata-sections

LDFLAGS = -T linker.ld -nostdlib -Wl,--gc-sections

TARGET = tinyrtos
SRCS = startup.c main.c

all: $(TARGET).bin

$(TARGET).elf: $(SRCS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f $(TARGET).elf $(TARGET).bin

.PHONY: all flash clean

