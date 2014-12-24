# Author: 		Ran Bao 
# Date:			30/Sept/2014
# Description:	Makefile for the assignment 2

# Define compilers and other flags
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -std=c99
LIBS = -Idrivers/avr -Idrivers -Ifonts -Iutils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm

# Default target
all: src/bouncing_ball.out

# Compile: create object files from C src
src/bouncing_ball.o: src/bouncing_ball.c drivers/avr/ir_uart.h drivers/avr/system.h drivers/display.h drivers/navswitch.h \
					fonts/font5x7_1.h utils/font.h utils/pacer.h utils/tinygl.h src/framebuffer.h drivers/button.h \
					src/key_event.h src/game.h src/task.h src/avatar.h src/misc.h drivers/led.h src/communication.h \
					src/random.h 
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/ir_uart.o: drivers/avr/ir_uart.c drivers/avr/ir_uart.h drivers/avr/pio.h drivers/avr/system.h drivers/avr/timer0.h drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/pio.o: drivers/avr/pio.c drivers/avr/pio.h drivers/avr/system.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/prescale.o: drivers/avr/prescale.c drivers/avr/prescale.h drivers/avr/system.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/system.o: drivers/avr/system.c drivers/avr/system.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/timer.o: drivers/avr/timer.c drivers/avr/system.h drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/timer0.o: drivers/avr/timer0.c drivers/avr/bits.h drivers/avr/prescale.h drivers/avr/system.h drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/usart1.o: drivers/avr/usart1.c drivers/avr/system.h drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/display.o: drivers/display.c drivers/avr/system.h drivers/display.h drivers/ledmat.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/ledmat.o: drivers/ledmat.c drivers/avr/pio.h drivers/avr/system.h drivers/ledmat.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/navswitch.o: drivers/navswitch.c drivers/avr/delay.h drivers/avr/pio.h drivers/avr/system.h drivers/navswitch.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/font.o: utils/font.c drivers/avr/system.h utils/font.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/pacer.o: utils/pacer.c drivers/avr/system.h drivers/avr/timer.h utils/pacer.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/tinygl.o: utils/tinygl.c drivers/avr/system.h drivers/display.h utils/font.h utils/tinygl.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/framebuffer.o: src/framebuffer.c drivers/avr/pio.h drivers/avr/system.h src/framebuffer.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/button.o: drivers/button.c drivers/avr/pio.h drivers/avr/system.h drivers/button.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/key_event.o: src/key_event.c drivers/avr/system.h drivers/button.h drivers/navswitch.h src/key_event.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/game.o: src/game.c drivers/avr/system.h src/game.h src/key_event.h src/framebuffer.h src/task.h src/avatar.h src/misc.h src/communication.h utils/tinygl.h fonts/font3x5_1.h src/misc.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/task.o: src/task.c drivers/avr/system.h drivers/avr/timer.h src/task.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/avatar.o: src/avatar.c src/avatar.h src/key_event.h src/framebuffer.h drivers/avr/system.h src/misc.h src/communication.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/misc.o: src/misc.c drivers/avr/system.h src/misc.h src/game.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@
	
src/led.o: drivers/led.c drivers/avr/system.h drivers/avr/pio.h drivers/led.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/communication.o: src/communication.c src/communication.h drivers/avr/ir_uart.h drivers/avr/system.h src/random.h src/avatar.h src/task.h drivers/led.h src/framebuffer.h src/game.h src/misc.h src/key_event.h src/game.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

src/random.o: src/random.c src/random.h drivers/avr/system.h drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

# Link
src/bouncing_ball.out: src/bouncing_ball.o src/ir_uart.o src/pio.o src/prescale.o src/system.o \
				src/timer.o src/timer0.o src/usart1.o src/display.o src/ledmat.o src/navswitch.o \
				src/font.o src/pacer.o src/tinygl.o src/framebuffer.o src/button.o \
				src/key_event.o src/game.o src/task.o src/avatar.o src/misc.o src/led.o src/communication.o \
				src/random.o
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ -lm
	$(SIZE) $@

# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) src/*.o src/*.out src/*.hex


# Target: program project.
.PHONY: program
program: src/bouncing_ball.out
	$(OBJCOPY) -O ihex src/bouncing_ball.out src/bouncing_ball.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash src/bouncing_ball.hex; dfu-programmer atmega32u2 start
