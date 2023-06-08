#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "LCD.h"
//#include "I2C.h"

char lcddata[20];
uint8_t keyout;
uint8_t user=0;

void additem();
uint8_t hx711H=0;
uint16_t hx711L=0;
volatile uint16_t TimerCal=0;
float loadCellRead();
#define Loadcell_data 5
#define Loadcell_clk 4

int main(void)
{
	LcdInit(0x27);
	LcdInit(0x27);//LCD initializing
	LcdSetCursor(4,0,"Welcome");
	_delay_ms(2000);
	
	additem();

}

float loadCellRead(){
	hx711H=0;hx711L=0;
	for(uint8_t i=0;i<8;i++){
		PORTC|=(1<<Loadcell_clk);
		_delay_us(10);
		if ((PINC&(1<<Loadcell_data))>>Loadcell_data)
		{hx711H|=(1<<(7-i));
		}
		else
		{hx711H&=~(1<<(7-i));
		}
		PORTC&=~(1<<Loadcell_clk);
		_delay_us(5);
	}
	
	
	for(uint8_t i=0;i<16;i++){
		PORTC|=(1<<Loadcell_clk);
		_delay_us(10);
		if ((PINC&(1<<Loadcell_data))>>Loadcell_data)
		{hx711L|=(1<<(15-i));
		}
		else
		{hx711L&=~(1<<(15-i));
		}
		PORTC&=~(1<<Loadcell_clk);
		_delay_us(5);
	}
	
	hx711L=hx711L>>1;
	
	if (hx711H&1)
	{hx711L|=(1<<15);
	}
	else
	{hx711L&=~(1<<15);
	}
	hx711H=hx711H>>1;
	
	return (hx711H*(65536/18029.6))+hx711L/18029.6;
}

void additem() {
	DDRC |= (1 << Loadcell_clk);
	PORTC &= ~(1 << Loadcell_clk);
	uint8_t itemkind = 0;
	
			while (1) {
				LcdSetCursor(0, 0, "Enter disposing ");
				LcdSetCursor(4, 1, "item");
				_delay_ms(500);
				while (1) {
					if (!(PINC & (1 << 2))) {
						break;
					}
				}
				LcdCommand(LCD_CLEARDISPLAY);
				LcdSetCursor(0, 0, "Calculating . .");
				uint16_t timeval = 0;
				while (1) {

					timeval++;
					sprintf(lcddata, "%u", timeval);
					LcdSetCursor(0, 1, lcddata);
					_delay_ms(500);
					if (!(PINC & (1 << 3))) {
						break;
					}
				}
				
				
				LcdCommand(LCD_CLEARDISPLAY);
				sprintf(lcddata, "length %u", timeval);
				LcdSetCursor(0, 0, lcddata);
				float weight = loadCellRead();
				sprintf(lcddata, "weight %0.0fg", weight);
				LcdSetCursor(0, 1, lcddata);
				_delay_ms(2000);
				
				LcdCommand(LCD_CLEARDISPLAY);
				if ((5 <= weight) && (weight <= 10) && (14 <= timeval) && (timeval <= 16)) 
				{
					itemkind = 1;
					eeprom_write_byte((uint8_t * )(user * 2), (eeprom_read_byte((uint8_t * )(user * 2))) + 1);
					LcdSetCursor(0, 0, "Pen Detected");
					_delay_ms(2000);
					/*TWIStart();
					TWIWriteAddrs(0x20,0);
					TWIWriteData(6);
					TWIStop();*/
					} 
					
					else if ((12 <= weight) && (weight <= 28) && (11 <= timeval) && (timeval <= 15)) 
					{
					itemkind = 2;
					eeprom_write_byte((uint8_t * )((user * 2) + 1), (eeprom_read_byte((uint8_t * )((user * 2) + 1))) + 1);
					LcdSetCursor(0, 0, "Highlighter Detected");
					_delay_ms(2000);
					/*TWIStart();
					TWIWriteAddrs(0x20,0);
					TWIWriteData(7);
					TWIStop();*/
					}
					 
					else {
					LcdSetCursor(0, 0, "Try again");
					_delay_ms(2000);
					/*TWIStart();
					TWIWriteAddrs(0x20,0);
					TWIWriteData(8);
					TWIStop();*/
				}
	}
}



