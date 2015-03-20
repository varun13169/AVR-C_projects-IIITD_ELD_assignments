#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<stdlib.h>
#include<string.h>

int button1=0;
int button2=0;
int button3=0;
int button4=0;
int button5=0;

int player_flag=1;
int count=0;
int flag=1;
int led_number;
int player1_count;
///////////////////


/////////////////////////////////////////////////////////////
#define rs PB0    //pin8 
#define rw PB1    //pin9
#define en PB2    //pin10




 

void lcdcmd(char cmdout)
{
	PORTD=cmdout;_delay_ms(10);
	PORTB &= ~(1<<rs);
	PORTB &= ~(1<<rw);
	PORTB |= (1<<en);
	_delay_ms(10);
	PORTB &= ~(1<<en);
	_delay_ms(10);	//delay so that lcd can execute the command
}

void lcddata(char dataout)
{
	PORTD = dataout;_delay_ms(10);
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

/////////////////////////////////////////////////////////////
int random_number();
void player1();
void player2();

int main(void)
{
    	DDRB |= 0x07;//0x07;
	DDRD |= 0xF0;//F0;
	DDRC = 0b00001111; // pins for led


	_delay_ms(500);
	start();
	_delay_ms(100);
	dis_cmd(0x80);
	Send_A_String("0");        // Initialize the LCD by writing Zero at (0,0)
	dis_cmd(0xC0);
	Send_A_String("0");        // Initialize the LCD by writing Zero at (1,0)



	EICRA = 1<<ISC10 | 1<<ISC00;
	EIMSK = 1<<INT0 | 1<<INT1; 
	PORTD |= 1<<PD3 | 1<<PD2;
	
	PCICR = 1<<PCIE0 | 1<<PCIE1 | 1<<PCIE2;
	PCMSK0 = 1<<PCINT3;
	
	PCMSK1 = 1<<PCINT13;
	
	
	PCMSK2 = 1<<PCINT17;
	
	sei();
	
	//while(flag==1){}
	if(1)
    	{
	    	count=0;
    		player1();player1();player1();player1();
    		dis_cmd(0x80);
    		Send_A_String("Player 1 score:");
    		dis_cmd(0xC0);
    		Send_An_Integer(count);
    		player1_count=count;
    		
    		count=0;
    		player2();player2();player2();player2();
        	dis_cmd(0x80);
    		Send_A_String("Player 2 score:");
    		dis_cmd(0xC0);
    		Send_An_Integer(count);
    		
    		_delay_ms(2000);
    		
    		if(player1_count > count)
    		{
    			dis_cmd(0x01);
    			Send_A_String("Player1 is");
    			dis_cmd(0xC0);
    			Send_A_String("faster");
    		}
    		else
    		{
    			dis_cmd(0x01);
    			Send_A_String("Player2 is");
    			dis_cmd(0xC0);
    			Send_A_String("faster");    		
    		}
	}
	return 0;
}



void player1()
{

	player_flag=1;
	led_number = random_number();
	while(player_flag==1)
	{
		_delay_ms(1);
		count++;
	}
	player_flag = 1;
	
}


void player2()
{

	player_flag=1;
	led_number = random_number();
	while(player_flag==1)
	{
		_delay_ms(1);
		count++;
	}
	player_flag = 1;
	
}

int random_number()
{
	int rand_value = rand()%4;
    		
  	switch(rand_value)
        {
        	case 0:
        		PORTC = 1<<PC0;break;
        	case 1:
        		PORTC = 1<<PC1;break;
        	case 2:
        		PORTC = 1<<PC2;break;
        	case 3:
        		PORTC = 1<<PC3;break;
        }
        return rand_value;

}

ISR(INT0_vect)
{
	button1++;
	if(button1%2==0)
	{
		PORTC ^= 1<<PC0;
		player_flag=0;
		_delay_ms(200);
		if(led_number!=0)
		{
			count+=1000;
			PORTC &= 0b11110000;
		}
	}
	_delay_ms(250);
}

ISR(INT1_vect)
{	
	button2++;
	if(button2%2==0)
	{
		PORTC ^= 1<<PC1;
		player_flag=0;
		_delay_ms(200);
		if(led_number!=1)
		{
			count+=1000;
			PORTC &= 0b11110000;
		}
	}
	_delay_ms(250);
}

ISR(PCINT0_vect)
{
	button3++;
	if(button3%2==0)
	{
		PORTC ^= 1<<PC2;
		player_flag=0;
		_delay_ms(200);
		if(led_number!=2)
		{
			count+=1000;
			PORTC &= 0b11110000;
		}
	}
	_delay_ms(250);
}

ISR(PCINT1_vect)
{	button4++;
	if(button4%2==0)
	{
		PORTC ^= 1<<PC3;
		player_flag=0;
		_delay_ms(200);
		if(led_number!=3)
		{
			count+=1000;
			PORTC &= 0b11110000;
		}
	}
	_delay_ms(250);
}

/*
ISR(PCINT2_vect)
{	button5++;
	if(button5%2==0)
	{
		flag=0;
	}
	_delay_ms(300);
}*/
