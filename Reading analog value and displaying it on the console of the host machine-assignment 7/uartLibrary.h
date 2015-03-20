/**
 * This library contains the interaction with the UART interface.
 * 
 * Usage:
 * 	1) In your program include "uartLibrary.h"
 * 	2) In your main program, have redefine
 * 	   stdout and stdin:
 * 	   		stdout = &uart_output;
 * 	   		stdin  = &uart_input;
 * 	3) If you successfully implemented the functions below,
 * 	   you will be able to use
 * 	   		puts(string s);
 * 	   		printf(string, parameters);
 * 	   to print data into the UART interface. With
 * 	   		char c = getChar();
 *
 * For further information have a look at the example code that is
 * included.
 */


#define F_CPU 16000000UL

#ifndef UART_LIBRARY_INCLUDED
#define UART_LIBRARY_INCLUDED

#define BAUD 9600
#define BRC ((F_CPU/16/BAUD)-1)


#include <util/setbaud.h>
#include <avr/io.h>
#include <stdio.h>

/**
 * Configures the UART port
 * Put all parameters that need to be set regarding the UART here.
 */
void uart_init() 
{
	UBRR0H = (BRC>>8); //Putting Upper 4 bits of BRC in lower 4 bits of UBRR0H
	UBRR0L = BRC; //Putting Lowwer 8 bits of BRC in UBRR0L

    	UCSR0B = 1 << TXEN0 | 1 << TXCIE0 | 1 << RXEN0 | 1 << RXCIE0;     //Enables TX, TX complete Interupt Enable, Enable RX, RX complete Interupt Enable
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);                               //8-bit data size
}


/**
 * Prints a character to the UART interface
 * @params c: the character to be printed
 * @params *stream: The file stream. Do not worry what
 * 		    is put there. It is used internally by puts
 * 		    or printf.
 */
void uart_putchar(char c, FILE *stream) 
{
	if (c == '\n')
	{
        	uart_putchar('\r', stream);
    	}
    	loop_until_bit_is_set(UCSR0A, UDRE0);  // Wait until data register empty.
    	UDR0 = c;

}


/**
 * Get a character from the UART interface
 * @params *stream: The file stream. Do not worry what
 * 		    is put there. It is used internally by puts
 * 		    or printf.
 */
char uart_getchar(FILE *stream)
{
    loop_until_bit_is_set(UCSR0A, RXC0); // Wait until data exists.
    return UDR0;
}


/**
 * Allows you to use printf, puts as commands to print something through the UART
 * on your serial console. It then can be displayed using HyperTerminal, minicom etc.
 */
FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

/**
 * Allows you to use getChar() to get a character from the user through UART
 */
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);


#endif
