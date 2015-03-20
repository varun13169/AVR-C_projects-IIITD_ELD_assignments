#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<string.h>
#include<stdlib.h>
#include"lcd.h"
//D4-D7 connected to D4-D7

#define rs PB0    //pin8 
#define rw PB1    //pin9
#define en PB2    //pin10




 

void lcdcmd(char cmdout)
{
	PORTD=cmdout;
	PORTB &= ~(1<<rs);
	PORTB &= ~(1<<rw);
	PORTB |= (1<<en);
	_delay_ms(10);
	PORTB &= ~(1<<en);
	_delay_ms(10);	//delay so that lcd can execute the command
}

void lcddata(char dataout)
{
	PORTD = dataout;
	PORTB |= (1<<rs);
	PORTB &= ~(1<<rw);
	PORTB |= (1<<en);
	_delay_ms(10);
	PORTB &= ~(1<<en);
	_delay_us(400);
}


void dis_cmd(char cmd_value)
{
	char cmd_value1;
	cmd_value1 = cmd_value & 0xF0;	//first 4 bits
	lcdcmd(cmd_value1); // send to LCD
	 
	cmd_value1 = ((cmd_value<<4) & 0xF0); //lower 4 bits
	lcdcmd(cmd_value1); // send to LCD
}
	 
void dis_data(char data_value)
{
	char data_value1;
	 
	data_value1=data_value & 0xF0;	//first 4 bits
	lcddata(data_value1);
	 
	data_value1=((data_value<<4) & 0xF0);	//lower 4 bits
	lcddata(data_value1);
}

	
void start()  
{
	dis_cmd(0x28);	//to initialize LCD in 2 lines, 5X8 dots and 4bit mode.
	dis_cmd(0x0C);	//display on cursor off. e for cursor on
	dis_cmd(0x06);	//entry mode-increment cursor by 1
	dis_cmd(0x01);	//clear screen
	dis_cmd(0x80);	//sets cursor to (0,0)
}

void Send_A_String(char *StringOfCharacters)
{
	while(*StringOfCharacters > 0)
	{
		dis_data(*StringOfCharacters++);
	}
}
 

void cut(char *str)
{
	int i=0;
	if(strlen(str)<16)
	{Send_A_String(str);}
	
	else
	{
		while(i<16)
		{
			dis_data(str[i]);
			i=i+1;
		}
		dis_cmd(0xC0);
		while(str[i]!='\0' && i<32)
		{
			dis_data(str[i]);
			i=i+1;
		}
		
	}	
}
void Send_An_Integer(int x)
{
	char buffer[16];
	itoa(x,buffer,10);
	cut(buffer);
}
