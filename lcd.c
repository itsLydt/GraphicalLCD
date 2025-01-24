/*
 * lcd.c
 *
 * Created: 1/24/2025 3:08:01 PM
 *  Author: itsLydt
 */ 

#include "lcd.h"
#include "ATSAMD21/drivers/gpio/gpio.h"

const uint8_t DB_MASK = 0xFF;
const uint8_t CTRL_MASK = (1 << PIN_RS) | (1 << PIN_RW) | (1 << PIN_E);

void write_command(uint8_t cmd);

void LCD_Init(){
	// configure GPIO pins that will be used to control the display
	GPIO_SetPortDirection(DATA_PORT, DB_MASK << PIN_DB0, GPIO_OUT);
	GPIO_SetPortDirection(CTRL_PORT, CTRL_MASK, GPIO_OUT);
	
	write_command(0x38);        //Function Set: 8-bit mode, basic instruction
	write_command(0x0C);        //Display control: Display on, cursor off, blink off
	write_command(0x06);		//Entry mode set
	write_command(0x02);		//Return home
	write_command(0x01);        //Display Clear
}

void write_command(uint8_t cmd){
	
	GPIO_WritePin(CTRL_PORT, PIN_RS, 1);			// RS = 0
	GPIO_WritePin(CTRL_PORT, PIN_RW, 1);			// Write; RW = 0
	GPIO_WritePin(CTRL_PORT, PIN_E, 0);				// E = 1
	GPIO_WriteValue(DATA_PORT, DB_MASK << PIN_DB0, cmd);	// write command data bus
	
	// hold data / E signal for 150ns
	//delay_ns(150);
	GPIO_WritePin(CTRL_PORT, PIN_E, 1);				// E = 0
	//delay_ns(20);	
}