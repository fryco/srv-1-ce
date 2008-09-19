OBJS=led.o timer.o motor.o laser.o uart.o pll.o sdram.o i2c.o camera.o syscalls.c srv-1-ce.o
SRCS=$(SRCS:%.o=%.c)

CROSS_COMPILE = bfin-elf-
CC = $(CROSS_COMPILE)gcc
LDR = $(CROSS_COMPILE)ldr

CPU = bf537-0.2
CFLAGS = -mcpu=$(CPU) 
CFLAGS += -W -Tbf537.ld -02
#CFLAGS += -W -Wall -Tbf537.ld -02
LDRFLAGS = --bits 16 --dma 8 --bmode para

all: srv-1-ce.ldr

%.ldr: %
	$(LDR) -T $(CPU) -c $(LDRFLAGS) $@ $^

load: srv-1-ce.ldr
	$(LDR) -l $< /dev/ttyS0

srv-1-ce: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	bfin-elf-objdump -d -S srv-1-ce > disassembly

clean:
	rm -f srv-1-ce *.o *.i *.s *.ldr disassembly

.PHONY: all clean load
