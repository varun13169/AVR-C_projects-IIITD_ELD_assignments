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
	
	dis_cmd(0x80);
	Send_A_String("0");        // Initialize the LCD by writing Zero at (0,0)
	dis_cmd(0xC0);
	Send_A_String("0");        // Initialize the LCD by writing Zero at (1,0)

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
	TCCR1B |= (1 << CS02);    //prescalar 256
	TCNT1 = 0;                //16 bit register
}

void t1()
{
	TCNT1=0;
	static int count1=0;
	static int flag1=0;    //It is there to toggle between the pattern given in the documentation
	int increment=0;        //It is used to increment count after every 1sec
	int task_switch;       //Its is used to count 5.5sec in order to shedule the task
	
	if(flag1==0){task_switch=0; flag1++;}
	else
	{
		flag1=0;
		task_switch=1;
		
		count1++;
		dis_cmd(0x80);
		Send_An_Integer(count1);
		
		while(TCNT1 < 31250){}  //It gives a delay of 0.5sec every alternate t1()
		TCNT1=0;
	}
	while(task_switch<11)            // Schedules task for 5.5secs 
	{
		if (TCNT1 >= 31250)
		{
			task_switch++;
			increment++;
			TCNT1=0;
		}
		if(increment==2)           // increment and print count1 every second
		{
			count1++;
			dis_cmd(0x80);
			Send_An_Integer(count1);
			increment=0;
		}
		
	}
}


void t2()
{
	TCNT1=0;
	static int count2=0;
	static int flag2=0;   //It is there to toggle between the pattern given in the documentation
	int increment=0;      // It is used to decrement count after every 2sec
	int task_switch;      //Its is used to count 5.5sec in order to shedule the task
		
	switch(flag2)
	{
		case 0:
			task_switch=0;
			flag2++;
			break;
		case 1:
			task_switch=1;
			flag2++;
			count2--;
			dis_cmd(0xC0);
			Send_An_Integer(count2);
			while(TCNT1 < 31250){} TCNT1=0;  //It gives a delay of 0.5sec every second t2()
			break;
		case 2:
			task_switch=2;
			flag2++;
			count2--;
			dis_cmd(0xC0);
			Send_An_Integer(count2);
			while(TCNT1 < 31250){} TCNT1=0;   //It gives a delay of 1sec every third t2()
			while(TCNT1 < 31250){} TCNT1=0;
			break;
		case 3:
			task_switch=3;
			flag2=0;
			count2--;
			dis_cmd(0xC0);
			Send_An_Integer(count2);
			while(TCNT1 < 31250){} TCNT1=0;   //It gives a delay of 1.5sec every fourth t2()
			while(TCNT1 < 31250){} TCNT1=0;
			while(TCNT1 < 31250){} TCNT1=0;
			break;
			
	}
	while(task_switch<11)                                // Schedules task for 5.5secs 
	{
		if (TCNT1 >= 31250)
		{
			task_switch++;
			increment++;
			TCNT1=0;
		}
		if(increment==4)                               // decrement and print count1 every two second
		{
			count2--;
			dis_cmd(0xC0);
			Send_An_Integer(count2);
			increment=0;
		}
		

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
