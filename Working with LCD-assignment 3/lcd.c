#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<string.h>

//D4-D7 connected to D4-D7
//3 CONTROLS:
#define rs PB0    //pin8 
#define rw PB1    //pin9
#define en PB2    //pin10
#define Data_Port PORTC

void start();		//fuction for intialize
void dis_cmd(char);	//divide and sends bits
void dis_data(char);	//divide and sends bits
void lcdcmd(char);	//manages the contols
void lcddata(char);	//manages the controls
void Send_A_String(char *StringOfCharacters);	//takes string as argument

int main()
{
	DDRB = 0x07;      //0b00000111
	DDRC = 0xFF;      //0b11111111
	DDRD = 0xFF;
	PORTD = 0x10;
	
	_delay_ms(2000);
	start();
	while(1)
	{	
		dis_cmd(0x80);
		Send_A_String("Varun Kumar");
		//dis_cmd(0xC0);	//Takes the cursor to 2nd line
		_delay_ms(2000);
		dis_cmd(0x01);	//clear screen
		dis_cmd(0x80);	
		Send_A_String("2013169");
		_delay_ms(2000);
		dis_cmd(0x01);
	}
	return 0;
}
 
void Send_A_String(char *StringOfCharacters)
{
	while(*StringOfCharacters > 0)
	{
		dis_data(*StringOfCharacters++);
	}
}
 	
void start()  
{
	dis_cmd(0x28);	//to initialize LCD in 2 lines, 5X8 dots and 4bit mode.
	dis_cmd(0x0C);	//display on cursor off. e for cursor on
	dis_cmd(0x06);	//entry mode
	dis_cmd(0x01);	//clear screen
	dis_cmd(0x80);	//sets cursor to 0,0
}

void dis_cmd(char cmd_value)
{
	char cmd_value1;
	
	cmd_value1 = cmd_value & 0xF0;	//first 4 bits
	cmd_value1 = cmd_value1 | 0x08;
	cmd_value1 = (cmd_value1>>2);
	lcdcmd(cmd_value1); // send to LCD
	 
	cmd_value1 = ((cmd_value<<4) & 0xF0); //lower 4 bits
	cmd_value1 = cmd_value1 | 0x08;
	cmd_value1 = (cmd_value1>>2);
	
	lcdcmd(cmd_value1); // send to LCD
}

/*void cut(char *po)
{
	int i=1;
	if(strlen(po)<=16){Send_A_String(po)}
	else
	{
		while(i<=16)
		{
			dis_data(po[i]);
		}
		dis_cmd(0xC0);
		while(po[i]!='\0' && i<=32)
		{
			dis_data(po[i]);
		}
		
	}	
}
*/	 
void dis_data(char data_value)
{
	char data_value1;
	 
	data_value1=data_value & 0xF0;	//first 4 bits
	data_value1=data_value1 | 0x08;
	data_value1=(data_value1>>2);
	
	lcddata(data_value1);
	 
	data_value1=((data_value<<4) & 0xF0);	//lower 4 bits
	data_value1=data_value1 | 0x08;
	data_value1=(data_value1>>2);
	lcddata(data_value1);
}

void lcdcmd(char cmdout)
{
	Data_Port=cmdout;
	PORTB &= ~(1<<rs);
	PORTB &= ~(1<<rw);
	PORTB |= (1<<en);
	_delay_ms(10);
	PORTB &= ~(1<<en);
	_delay_ms(10);	//delay so that lcd can execute the command
}

void lcddata(char dataout)
{
	Data_Port = dataout;
	PORTB |= (1<<rs);   //PORTB=PORTB | (1<<rs)
	PORTB &= ~(1<<rw);
	PORTB |= (1<<en);
	_delay_ms(10);
	PORTB &= ~(1<<en);
	_delay_us(400);
}
