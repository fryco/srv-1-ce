CSRCS = led.c timer.c motor.c laser.c blob.c uart.c pll.c sdram.c i2c.c jpeg.c camera.c syscalls.c srv-1-ce.c
ASMSRCS = r8x8dct.S
OBJS = $(CSRCS:%.c=%.o) $(ASMSRCS:%.S=%.o)

CROSS_COMPILE = bfin-elf-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
LDR = $(CROSS_COMPILE)ldr

CPU = bf537-0.2
CFLAGS = -mcpu=$(CPU) -W -Wall -O2
ASMFLAGS = $(CFLAGS)
LFLAGS = -Tbf537.ld
LDRFLAGS = --bits 16 --dma 8 --bmode para

all: srv-1-ce.ldr

%.ldr: %
	$(LDR) -T $(CPU) -c $(LDRFLAGS) $@ $^

srv-1-ce: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o srv-1-ce
	
%.o: %.S
	$(CC) $(ASMFLAGS) -c $<

clean:
	rm -f srv-1-ce *.o *.i *.ldr disassembly

load:
	 bfin-elf-ldr -l srv-1-ce.ldr 192.168.0.100:10001
.PHONY: all clean load
	




