/*
 * lcd.c
 *
 * Created: 1/24/2025 3:08:01 PM
 *  Author: itsLydt
 */ 

#include "lcd.h"
#include "config.h"
#include "delay.h"

const uint8_t DB_MASK = 0xFF;
#define CTRL_MASK ((1 << PIN_RS) | (1 << PIN_RW) | (1 << PIN_E))

void write_command(uint8_t cmd);

void LCD_Init(){
	// configure GPIO pins that will be used to control the display
	GPIO_SetPortDirection(DATA_PORT, DB_MASK << PIN_DB0, GPIO_OUT);
	GPIO_SetPortDirection(CTRL_PORT, CTRL_MASK, GPIO_OUT);
	
	write_command(0x38);        //Function Set: 8-bit mode, basic instruction
	SetDisplayMode(1, 0, 0);	// Display on, cursor off, blink off
	SetEntryMode(1, 0);			// Cursor increment, no shift
	ReturnHome();
	ClearDisplay();
}

void write_command(uint8_t cmd){
	
	GPIO_WritePin(CTRL_PORT, PIN_RS, 1);			// RS = 0
	GPIO_WritePin(CTRL_PORT, PIN_RW, 1);			// Write; RW = 0
	GPIO_WritePin(CTRL_PORT, PIN_E, 0);				// E = 1
	GPIO_WriteValue(DATA_PORT, DB_MASK << PIN_DB0, cmd);	// write command data bus
	
	// hold data / E signal for 150ns
	delay_ns(150);
	GPIO_WritePin(CTRL_PORT, PIN_E, 1);				// E = 0
	delay_ns(20);	
}

void write_data(uint8_t data){
	
	GPIO_WritePin(CTRL_PORT, PIN_RS, 0);			// RS = 1
	GPIO_WritePin(CTRL_PORT, PIN_RW, 1);			// Write; RW = 0
	GPIO_WritePin(CTRL_PORT, PIN_E, 0);				// E = 1
	GPIO_WriteValue(DATA_PORT, DB_MASK << PIN_DB0, data);	// write command data bus
	
	// hold data / E signal for 150ns
	delay_ns(150);
	GPIO_WritePin(CTRL_PORT, PIN_E, 1);				// E = 0
	delay_us(72);
}

void ClearDisplay() {
	write_command(0x01);
	delay_us(1600);		// command takes 1.6ms to execute
}

void ReturnHome(){
	write_command(0x02);
	delay_us(72);		// command takes 72 us
}

void SetEntryMode(_Bool cursor_increment, _Bool shift_display){
	uint8_t cmd = 0x04;
	cmd |= (cursor_increment << 1);
	cmd |= (shift_display);
	write_command(cmd);
	delay_us(72);
}

/* Set Display Mode: display on/off, cursor on/off, character blink on/off */
void SetDisplayMode(_Bool enable_display, _Bool enable_cursor, _Bool enable_blink){
	uint8_t cmd = 0x08;
	cmd |= (enable_display << 2);
	cmd |= (enable_cursor << 1);
	cmd |= (enable_blink);
	delay_us(72);
}

void FillWith(uint8_t val)
{
	unsigned int x, page;
	ReturnHome();
	for (page=0x80;page<0xA1;page++)
	{
		write_command(0x3E);
		write_command(page);
		write_command(0x80);
		for (x=0;x<18;x++)
		{
			write_data(val);
		}
	}
}

void FillRowWith(uint8_t row, uint8_t val){
	write_command(0x3E);
	delay_us(72);
	write_command(row);	// write vertical address
	delay_us(72);
	write_command(0x80);	// write horizontal address
	delay_us(72);
	for (int x = 0;x<18;x++)
	{
		write_data(val);
	}
}

void Smile(){
	unsigned int page = 0x80;
	for (int i = 0; i < 5; i++)
	{
		FillRowWith(page++, 0x00);
		FillRowWith(page++, 0x24);
		FillRowWith(page++, 0x00);
		FillRowWith(page++, 0x42);
		FillRowWith(page++, 0x3C);
		FillRowWith(page++, 0x00);
	}
	
	FillRowWith(page++, 0x00);
	FillRowWith(page++, 0x00);
}

void CountingPattern(){
	for (uint8_t page = 0x80; page<0xA1; page++){
		write_command(0x3E);
		delay_us(72);
		write_command(page);
		delay_us(72);
		write_command(0x80);
		delay_us(72);

		for(int x = 0; x < 18; x++){
			write_data(x);
		}
	}
}