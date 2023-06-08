#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "LCD.h"
#include "I2C.h"


const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t getkeyNum();
char lcddata[20];
uint8_t keyout;
uint8_t user=0;


void additem();

void stepper(uint16_t rev,uint8_t dir);

int main(void)
{
	
	TWIInit();
	DDRD|=0xF0;
	LcdInit(0x27);
	LcdSetCursor(4,0,"Welcome");
	_delay_ms(2000);
	
	for(uint8_t i=0;i<10;i++){
		//eeprom_write_byte((uint8_t*)i,0);
	}
	
	additem();
	
	
	while (1)
	{
		
	}
}

uint8_t getkeyNum(){
	DDRA=0b00001111;
	PORTA=0b11110000;
	uint8_t getx;
	uint8_t gety;
	while(1){
		
		uint8_t pin=(~(PINA|0x0F));
		if (pin)
		{
			
			
			switch(pin){
				
				
				case (1<<4) :getx=0; break;
				case (1<<5) :getx=1;break;
				case (1<<6) :getx=2;break;
				case (1<<7) :getx=3;break;
				
				
			} //switch
			DDRA=0b0;
			PORTA=0b0;
			
			DDRA=0b11110000;
			PORTA=0b00001111;
			
			pin=~(PINA|0xF0);
			
			switch(pin){
				case (1<<0) :gety=0;break;
				case (1<<1) :gety=1;break;
				case (1<<2) :gety=2;break;
				case (1<<3) :gety=3;break;
				
			} //switch
			
			DDRA=0b00001111;
			PORTA=0b11110000;
			
			
			return kaypad[gety][getx];
			
		}//if
		
		
		
	}//while 1
}

void additem(){
	uint8_t itemkind=0;
	
	while(1){
	LcdSetCursor(0,1,"pen-1 highlight-2");
	LcdSetCursor(4,1,"Try Again-3");
	keyout=20;
	keyout=getkeyNum();
	
	if (keyout==1)
	{
		itemkind=1;
		eeprom_write_byte((uint8_t*)(user*2),(eeprom_read_byte((uint8_t*)(user*2)))+1);
		LcdSetCursor(0,0,"Pen Detected");
		TWIStart();
		TWIWriteAddrs(0x20,0);
		TWIWriteData(6);
		TWIStop();
	}
	else if (keyout==2)//12-28g,11-15cm
	{
		itemkind=2;
		eeprom_write_byte((uint8_t*)((user*2)+1),(eeprom_read_byte((uint8_t*)((user*2)+1)))+1);
		LcdSetCursor(0,0,"Highlighter Det.");
		TWIStart();
		TWIWriteAddrs(0x20,0);
		TWIWriteData(7);
		TWIStop();
	}
	else if(keyout==3){
		LcdSetCursor(0,0,"Try again");
		TWIStart();
		TWIWriteAddrs(0x20,0);
		TWIWriteData(8);
		TWIStop();
	}
	
		LcdSetCursor(0,1,"ReEnter-1 Exit-2");
		keyout=20;
		keyout=getkeyNum();
		if (keyout==1)
		{LcdCommand(LCD_CLEARDISPLAY);
		}
		if (keyout==2)
		{	LcdCommand(LCD_CLEARDISPLAY);
			break;
		}
}
		
	if (itemkind==1)
	{
			LcdSetCursor(0,0,"Select pen");
			LcdSetCursor(0,1,"Blue-1 Black-2 Red-3");
			while(1){
				keyout=getkeyNum();
				if ((keyout==1)||(keyout==2)||(keyout==3))
				{
					TWIStart();
					TWIWriteAddrs(0x20,0);
					TWIWriteData(keyout);
					TWIStop();
					break;
				}
			}
			
			stepper(5,1);
			while(1){
				if (!(PINC&(1<<6)))
				{
					TWIStart();
					TWIWriteAddrs(0x20,0);
					TWIWriteData(0);
					TWIStop();
					break;
				}
			}
			
			stepper(5,0);
		
		/*else{
			LcdSetCursor(0,0,"Not enough");
			LcdSetCursor(0,1,"Balance");
		}*/
		
	}
	else if (itemkind==2)
	{
			LcdSetCursor(0,0,"Select type");
			LcdSetCursor(0,1,"Green-1 Yellow-2");
			
			while(1){
				keyout=getkeyNum();
				if ((keyout==1)||(keyout==2))
				{
					TWIStart();
					TWIWriteAddrs(0x20,0);
					TWIWriteData(keyout+3);
					TWIStop();
					break;
				}
			}
			
			stepper(5,1);
			while(1){
				if (!(PINC&(1<<6)))
				{
					TWIStart();
					TWIWriteAddrs(0x20,0);
					TWIWriteData(0);
					TWIStop();
					break;
				}
			}
			
			stepper(5,0);
		/*else{
			LcdSetCursor(0,0,"Not enough");
			LcdSetCursor(0,1,"Balance");
		}*/
		
	}
	
}

void stepper(uint16_t rev,uint8_t dir)
{
	for(uint16_t j=0;j<rev;j++)
	{
		
		
		if (dir)
		{for(uint8_t i=4;i<8;i++)
			{
				PORTD=(PORTD&0x0F)|(1<<i);
				_delay_ms(100);
				if (i==7)
				{
					PORTD|=(1<<4);
				}
				else
				{
					PORTD|=(1<<i+1);
				}
				_delay_ms(100);
			}
		}
		else
		{
			for(uint8_t i=7;i>3;i--){
				PORTD=(PORTD&0x0F)|(1<<i);
				_delay_ms(100);
				if (i==7)
				{PORTD|=(1<<7);
				}
				else
				{PORTD|=(1<<i-1);
				}
				_delay_ms(100);
			}
		}
		
		
		
	}
	
}

