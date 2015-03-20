#include "uartLibrary.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
#define F_CPU       16000000UL
#define BAUD        9600
#define BRC         ((F_CPU/16/BAUD) - 1)       
*/
#define LDR_value   50                          //critical value of LED
#define NTC_value   125                        //critical value of NTC


uint8_t flags= 0x00;                            //first bit is for LDR, second bit is for NTC

void setup_adc();

int main(void)
{  
	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;
	
	DDRD = 0xA0;                             //Set Pin5 and Pin7 on arduino as output
	
	sei();                                   //Enales global interrupt
	setup_adc();                             //Setup ADC according to the defined function
	
	while(1)
	{
	}
}
 

void setup_adc()
{
	ADCSRA |= 1<<ADEN;                       //Enables ADC
	ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;//Sets ADC Prescalar as 128, i.e. my ADC frequency is 125KHz
	ADCSRA |=  1<<ADSC | 1<<ADIE;            //Enables ADC interupt and Start the conversion
	ADMUX |= 1<<ADLAR | 1<<REFS0;            //ADLAR=1 for left adjusted result and REFS0=1 with REFS1=0 to use Vcc as reference voltage
	DIDR0 |= 1<<ADC1D | 1<<ADC0D;            //disabling digital input of PC0,PC1 pins to save power
}


ISR(USART_RX_vect)
{
	if(UDR0=='a')
	{
		switch(flags)
		{
			case 0x00:
				printf("LED LDR: OFF\t");printf("LED temp: OFF\n\n");break; //0b00000000
			case 0x01:
				printf("LED LDR: ON\t");printf("LED temp: OFF\n\n");break;  //0b00000001
			case 0x02:
				printf("LED LDR: OFF\t");printf("LED temp: ON\n\n");break;  //0b00000010
			case 0x03:
				printf("LED LDR: ON\t");printf("LED temp: ON\n\n");break;  //0b00000011
		}
	}
	else if(UDR0!='a'){printf("Invalid key pressed.\n\n");}
_delay_ms(5000);
}	


ISR(ADC_vect)
{
	uint8_t adcl = ADCL;                             //This is an 8-bit varible used to store the value of ADLC
	uint16_t adc_ten_bit_value = ADCH<<2 | adcl>>6;  //This is an 16-bit varible use to store the 10-bit value of left adjusted result
	
	int value_of_mux0= ADMUX & 1<<MUX0;
	
	if(adc_ten_bit_value>NTC_value && (value_of_mux0==0)){PORTD|=1<<PD7; flags|=0x02;}         //NTC
	else if(value_of_mux0==0){PORTD&=(~(1<<PD7)); flags&=0xFD;}
	//Input from NTC is on A0, hence when value_of_mux0 is equal to zero and the recieved adc_ten_bit_value is greater than 133, then make Pin7 on arduino HIGH else make it LOW
	
	
	if(adc_ten_bit_value<LDR_value && (value_of_mux0!=0)) {PORTD|=1<<PD5; flags|=0x01;}         //LDR
	else if(value_of_mux0!=0){PORTD&=(~(1<<PD5)); flags&=0xFE;}
	//Input from LDR is on A1, hence when value_of_mux0 is'nt equal to zero and the recieved adc_ten_bit_value is less than ten, then make Pin5 on arduino HIGH else make it LOW
	
	
	ADMUX ^= 1<<MUX0;     //Toggles one of the select bit MUX0, i.e. toggling takes place in between A0 and A1 pin on arduino
	ADCSRA |= 1<<ADSC;    //Starts the conversion again
}
