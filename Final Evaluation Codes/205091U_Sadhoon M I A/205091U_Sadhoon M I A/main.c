#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void init_slave(void);
uint8_t read_data(void);
void slave_listen(void);

unsigned char recv_data;
void servoangle(uint8_t pin);
int main(void)
{

	DDRA=0xFF;
	DDRB=0xFF;
	init_slave();

	
	while(1)
	{
		
		slave_listen();
		read_data();
		
		//for DC motor
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
