proj: diode.c 
		clear
		avr-gcc -g -Os -mmcu=atmega328 -c -std=c99 diode.c  
		avr-gcc -g -std=c99 -Os -mmcu=atmega328 -o diode.elf diode.o 
		avr-objcopy -j .text -j .data -O ihex diode.elf diode.hex
