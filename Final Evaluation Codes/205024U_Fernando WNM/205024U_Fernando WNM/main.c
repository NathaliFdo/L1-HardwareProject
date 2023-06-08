#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "USART.h"

volatile uint16_t TimerCal=0;
uint16_t ultraLeftP=0;
uint16_t ultraRightH=0;
char sendData[30];
	
int main(void)
{
    USART_Init(9600);
	
	/*Ultrasonic Timer Part*/
	DDRC|=(1<<2); 
	TCCR0|=(1<<WGM01);
	TCCR0|=(1<<CS11);
	TCNT0=0;
	OCR0=10;
	
	/*register value= time*(clock speed/prescale)
	register value=0.000001*(8000000/1)
	register value=10*/
	
	TIMSK|=(1<<OCIE0);
	sei(); 
	
	
    while (1) 
    {
		PORTC&=~(1<<2);
		_delay_us(50);
		PORTC|=(1<<2);
		_delay_us(50);
		PORTC&=~(1<<2);
		
		while(!(PINC&(1<<3)))
			TimerCal=0;
		while((PINC&(1<<3)))
			ultraLeftP=TimerCal/4.1375;
		
		_delay_ms(50);
		
		PORTC&=~(1<<2);
		_delay_us(50);
		PORTC|=(1<<2);
		_delay_us(50);
		PORTC&=~(1<<2);
		
		while(!(PINC&(1<<4)))
			TimerCal=0;
		while((PINC&(1<<4)))
			ultraRightH=TimerCal/4.1375;
		
		if ((ultraLeftP<50)&&(ultraRightH>49))
		{
			sprintf(sendData,"Pens bin is filled");
			
		}
		else if ((ultraRightH<50)&&(ultraLeftP>49)){
			sprintf(sendData,"Highlighters bin is filled");
		}
		else if ((ultraRightH<50)&&(ultraLeftP<50)){
			sprintf(sendData,"Bins are filled");
		}
		_delay_ms(50);
	
	
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT+CMGF=1");
	_delay_ms(500);
	USART_TxStringln("AT+CMGS=\"+94772562509\"");
	_delay_ms(500);
	USART_TxStringln(sendData);
	_delay_ms(500);
	USART_Transmit(26);
	_delay_ms(30000);
	
}
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
}


