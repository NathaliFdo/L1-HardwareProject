// Program for Slave mode
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
//#include <stdio.h>
//#include "USART.h"

//void sendMessage(char sendMsg[30]);
void init_slave(void);
uint8_t read_data(void);
void slave_listen(void);

/*volatile uint16_t TimerCal=0;// variable for collect echo data
uint16_t ultraLeftP=0;
uint16_t ultraRightH=0;
char sendData[30];*/

unsigned char recv_data;
void servoangle(uint8_t pin);
int main(void)
{
	//USART_Init(9600);
	DDRA=0xFF;
	DDRB=0xFF;
	init_slave();
	
	/*Ultrasonic Timer Part
	DDRC|=(1<<2); 
	TCCR0|=(1<<WGM01);
	TCCR0|=(1<<CS11);
	TCNT0=0;
	OCR0=10;*/
	
	/*register value= time*(clock speed/prescale)
	register value=0.000001*(8000000/1)
	register value=10*/
	
	/*TIMSK|=(1<<OCIE0);
	sei();*/
	
	//DDRB|=(1<<0);
	while (1)
	{
		/*PORTC&=~(1<<2);
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
		{//PORTB|=(1<<0);
			sprintf(sendData,"Pens bin is filled");
			
		}
		else if ((ultraRightH<50)&&(ultraLeftP>49)){
			sprintf(sendData,"Highlighters bin is filled");
		}
		else if ((ultraRightH<50)&&(ultraLeftP<50)){
			sprintf(sendData,"Bins are filled");
		}
		//else
		//{//PORTB&=~(1<<0);
		//}
		//_delay_ms(50);*/
		
		slave_listen();
		read_data();
		
		if (recv_data<6)
		{PORTA|=1<<(recv_data+1);
			if (recv_data==0)
			{PORTA=0;
			}
		}
		else{
			servoangle(recv_data-6);
		}
		_delay_ms(500);
		
	}
		
	
}

void init_slave(void)
{
	TWAR=0x20;
}

void slave_listen(void)
{
	TWCR=(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}


uint8_t read_data(void)
{
	TWCR=(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	recv_data=TWDR;
	return TWDR;
}

void servoangle(uint8_t pin){
	
	for(uint8_t j=0;j<100;j++){
		PORTB|=(1<<pin);
		for(uint8_t i=0;i<10;i++){
			_delay_us(100);
		}
		PORTB&=~(1<<pin);
		
		
		for(uint8_t i=0;i<10;i++){
			_delay_us(100);
		}
	}
	_delay_ms(500);
	for(uint8_t j=0;j<100;j++){
		
		PORTB|=(1<<pin);
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
		PORTB&=~(1<<pin);
		
		
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
	}
}

/*void sendMessage(char sendMsg[30]){
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT+CMGF=1");
	_delay_ms(500);
	USART_TxStringln(sendMsg);
	_delay_ms(500);
	USART_Transmit(26);
	_delay_ms(300);
}


ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}*/

