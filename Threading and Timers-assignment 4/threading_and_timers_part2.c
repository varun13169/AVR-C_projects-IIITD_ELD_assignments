#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<stdlib.h>
#include<string.h>
#include"lcd.h"

void timer();
void t1();
void t2();


int main(void)
{
	

	timer();
    	DDRB = 0x07;
	DDRD = 0xF0;
	_delay_ms(500);
	start();
	
	dis_cmd(0x80);            // Initialize the LCD by writing Zero at (0,0)
	Send_A_String("0");
	dis_cmd(0xC0);            // Initialize the LCD by writing Zero at (1,0)
	Send_A_String("0");

	while(1)
    	{
        	t1();
		t2();
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////
void timer()
{
	TCCR1B |= (1 << CS02);     //prescaler 256
	TCNT1 = 0;                    //16bit register
}


void t1()
{
	TCNT1=0;
	static int count1=0;             //Stores the incremented value
	static int increment1=0;         //Counts till 4, then increment count1
	int loop = 0;
	
	while(loop<1)                    //For scheduling in 250ms
	{
		if (TCNT1 >= 15625)
		{
			increment1++;
			TCNT1=0;
			loop++;
		}
		if(increment1%4==0 && increment1!=0)    //increments count1 after 1.75sec
		{
			count1++;
			dis_cmd(0x80);
			Send_An_Integer(count1);
			increment1=0;
		}
		
	}
}


void t2()
{
	TCNT1=0;
	static int count2=0;                  //Stores the incremented value
	static int increment2=0;              //Counts till 4, then increment count2
	int loop=0;
	
	while(loop<1)                           //For scheduling in 250ms
	{
		if (TCNT1 >= 15625)
		{
			increment2++;
			TCNT1=0;
			loop++;
		}
		if(increment2%8==0 && increment2!=0)          //increments count2 after 4sec
		{
			count2--;
			dis_cmd(0xC0);
			Send_An_Integer(count2);
			increment2=0;
		}
		

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////

