proj: motor.c 
		clear
		avr-gcc -g -Os -mmcu=atmega328p -c -std=c99 motor.c  
		avr-gcc -g -std=c99 -Os -mmcu=atmega328p -o motor.elf motor.c 
		avr-objcopy -j .text -j .data -O ihex motor.elf motor.hex
