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

void LCD_Init(){
	// configure GPIO pins that will be used to control the display
	GPIO_SetPortDirection(DATA_PORT, DB_MASK << PIN_DB0, GPIO_OUT);
	GPIO_SetPortDirection(CTRL_PORT, CTRL_MASK, GPIO_OUT);
}