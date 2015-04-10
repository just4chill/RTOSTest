
CC = arm-none-eabi-

INC = 	-Ikernel/include \
		-Ikernel/portable/GCC/ARM_CM3 \
		-Idevice \
		-Iconfig \
		-Idrivers \
		-Icmsis

VPATH = kernel: \
		kernel/portable/GCC/ARM_CM3: \
		kernel/portable/MemMang: \
		app: \
		device: \
		drivers: \
		syscalls:\
		startup:\
		cmsis

LDSCRIPT = linker.ld

CFLAGS = -g -c -Wall -Os -mthumb -mcpu=cortex-m3 $(INC) -std=c99
LFLAGS = -g -Wall -Os -mthumb -mcpu=cortex-m3 -nostartfiles --specs=nano.specs -T$(LDSCRIPT)

OUTPUT = app.elf

OBJS = 	list.o \
		queue.o \
		tasks.o \
		timers.o \
		port.o \
		heap_1.o \
		lpc17xx.o \
		system_LPC17xx.o \
		main.o \
		syscalls.o \
		lpc17xx_uart.o \
		lpc17xx_clkpwr.o \
		lpc17xx_libcfg_default.o \
		uart_interrupt.o \
		lpc17xx_pinsel.o

all: $(OBJS)
	$(CC)gcc $(LFLAGS) $(OBJS) -o $(OUTPUT)
	$(CC)size $(OUTPUT)

clean: $(OBJS)
	-rm $(OBJS)

list.o: list.c
	$(CC)gcc $(CFLAGS) $^ -o $@

queue.o: queue.c
	$(CC)gcc $(CFLAGS) $^ -o $@

tasks.o: tasks.c
	$(CC)gcc $(CFLAGS) $^ -o $@

timers.o: timers.c
	$(CC)gcc $(CFLAGS) $^ -o $@

port.o: port.c
	$(CC)gcc $(CFLAGS) $^ -o $@

heap_1.o: heap_1.c
	$(CC)gcc $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC)gcc $(CFLAGS) $^ -o $@

lpc17xx.o: lpc17xx.S
	$(CC)gcc $(CFLAGS) $^ -o $@

system_LPC17xx.o: system_LPC17xx.c
	$(CC)gcc $(CFLAGS) $^ -o $@

syscalls.o: syscalls.c
	$(CC)gcc $(CFLAGS) $^ -o $@

lpc17xx_uart.o: lpc17xx_uart.c
	$(CC)gcc $(CFLAGS) $^ -o $@

lpc17xx_clkpwr.o: lpc17xx_clkpwr.c
	$(CC)gcc $(CFLAGS) $^ -o $@
	
lpc17xx_libcfg_default.o: lpc17xx_libcfg_default.c
	$(CC)gcc $(CFLAGS) $^ -o $@

uart_interrupt.o: uart_interrupt.c
	$(CC)gcc $(CFLAGS) $^ -o $@
	
lpc17xx_pinsel.o: lpc17xx_pinsel.c
	$(CC)gcc $(CFLAGS) $^ -o $@

