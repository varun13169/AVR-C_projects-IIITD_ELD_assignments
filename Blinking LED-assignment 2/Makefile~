f = reaction_testing_game
comp = avr-gcc
p = /dev/ttyUSB*
pro = -mmcu=atmega328p

.PHONY:all compile download clean

all: compile download uartLibrary.h
	
compile: $(f).c uartLibrary.h
	$(comp) -g $(pro) -Wall -Os $(f).c -o $(f).elf		#makes .elf
	avr-objcopy -j .text -j .data -O ihex $(f).elf $(f).hex		#makes hex

download: $(f).hex
	sudo avrdude -F  -V -c arduino -p m328  -P $(p) -b 57600 -e -U flash:w:$(f).hex  #uploads the code

clean:
	rm -f *.elf *.hex *.o
