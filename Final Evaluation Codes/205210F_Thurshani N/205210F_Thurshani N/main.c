#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"

const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t getkeyNum();
char lcddata[20];
uint8_t keyout;
uint8_t user=0;

uint8_t enterPassword();
void additem();

const uint8_t password[5][6]={{7,8,9,1,2,3},{4,5,6,1,8,9},{1,2,3,2,9,8},{3,0,4,5,6,8},{9,8,7,5,3,1}};

int main(void)
{
	
	
	DDRD|=0xF0;
	LcdInit(0x27);
	LcdSetCursor(4,0,"Welcome");
	_delay_ms(2000);
	
	for(uint8_t i=0;i<10;i++){
		//eeprom_write_byte((uint8_t*)i,0);
	}
	
	
	
	while(1){
		if (enterPassword())
		{break;
		}
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


uint8_t enterPassword(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Enter password");
	
	_delay_ms(500);
	uint8_t pos=0;
	uint8_t numbers[6]={0,0,0,0};
	sprintf(lcddata,"%u%u%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3],numbers[4],numbers[5]);
	LcdSetCursor(0,1,lcddata);
	LcdSetCursor(pos,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);
	
	while(1){
		keyout=getkeyNum();
		if (keyout<10)
		{
			numbers[pos]=keyout;
			sprintf(lcddata,"%u%u%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3],numbers[4],numbers[5]);
			LcdSetCursor(0,1,lcddata);
			pos++;
			if (pos>5)
			{pos=0;
			}
			LcdSetCursor(pos,1,"");
			_delay_ms(200);
		}
		
		else if (keyout==14)
		{LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF);
			break;
		}
	}
	
	uint8_t outval=0;
	
	for(uint8_t i=0;i<5;i++){
		
		outval=0;
		for(uint8_t j=0;j<6;j++){
			if (password[i][j]==numbers[j])
			{outval++;
			}
		}
		
		if (outval>=6)
		{	user=i;
			break;
		}
		
		
	}
	
	LcdCommand(LCD_CLEARDISPLAY);
	
	if (outval>=6)
	{LcdSetCursor(0,0,"Password ok");
		_delay_ms(500);
		LcdCommand(LCD_CLEARDISPLAY);
		return 1;
	}
	else
	{LcdSetCursor(0,0,"Password error");
		_delay_ms(500);
		LcdCommand(LCD_CLEARDISPLAY);
		return 0;
	}	
}

void additem(){
	uint8_t itemkind=0;
	
	while(1){
	LcdSetCursor(0,0,"pen-1 highlight-2");
	LcdSetCursor(4,1,"Try Again-3");
	keyout=20;
	keyout=getkeyNum();
	
	if (keyout==1)
	{
		itemkind=1;
		
		LcdSetCursor(0,0,"Pen Detected");
		
	}
	else if (keyout==2)//12-28g,11-15cm
	{
		itemkind=2;
		
		LcdSetCursor(0,0,"Highlighter Det.");
	}
	else if(keyout==3){
		LcdSetCursor(0,0,"Try again");
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
		/*else{
			LcdSetCursor(0,0,"Not enough");
			LcdSetCursor(0,1,"Balance");
		}*/
		
	}
	else if (itemkind==2)
	{
			LcdSetCursor(0,0,"Select type");
			LcdSetCursor(0,1,"Green-1 Yellow-2");
						
		/*else{
			LcdSetCursor(0,0,"Not enough");
			LcdSetCursor(0,1,"Balance");
		}*/
		
	}
	
}
