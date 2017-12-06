#makefile config
CPU		= msp430g2553
CFLAGS		= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/

#switch the compiler
CC		= msp430-elf-gcc
AS		= msp430-elf-gcc -mmcu=${CPU} -c

all: p3main.elf

#additional rules for files
p3main.elf: ${COMMON_OBJECTS} p3main.o wdt_handler.o main_menu.o wdtHandler.o level1.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd -lShape -lCircle -lp2sw

load: p3main.elf
	mspdebug rf2500 "prog $^"

clear:
	rm -f *.o *.elf
